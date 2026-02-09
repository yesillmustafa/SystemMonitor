#include "ProcessMonitor.h"
#include "Logger.h"
#include "FormatUtils.h"
#include <Psapi.h>
#include <unordered_set>
#include <chrono>

ProcessMonitor::ProcessMonitor(int intervalSeconds)
	: m_intervalSeconds(intervalSeconds)
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    m_cpuCoreCount = sysInfo.dwNumberOfProcessors;

    Logger::GetInstance().Log(
        "ProcessMonitor initialized. CPU cores: " + std::to_string(m_cpuCoreCount),
        LogLevel::DEBUG
    );
}

ProcessMonitor::~ProcessMonitor()
{
    Stop();
}

void ProcessMonitor::Start()
{
    if (m_running)
        return;

    m_running = true;
    m_worker = std::thread(&ProcessMonitor::WorkerLoop, this);

    Logger::GetInstance().Log("ProcessMonitor thread started", LogLevel::DEBUG);
}

void ProcessMonitor::Stop()
{
    if (!m_running)
        return;

    m_running = false;
    m_cv.notify_all();

    if (m_worker.joinable())
        m_worker.join();

    Logger::GetInstance().Log("ProcessMonitor thread stopped", LogLevel::DEBUG);
}

void ProcessMonitor::WorkerLoop()
{
    std::unique_lock<std::mutex> lock(m_cvMutex);

    while (m_running)
    {
        Update();

        m_cv.wait_for(lock, std::chrono::seconds(m_intervalSeconds), [&]() {
            return !m_running;
        });
    }
}

void ProcessMonitor::Update()
{
    std::vector<ProcessInfo> newProcessList;
    std::unordered_map<DWORD, CpuHistory> newCpuHistory;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        Logger::GetInstance().Log("Process snapshot could not be taken", LogLevel::ERR);
        return;
    }

    FILETIME sysIdle, sysKernel, sysUser;
    if (!GetSystemTimes(&sysIdle, &sysKernel, &sysUser))
    {
        Logger::GetInstance().Log("GetSystemTimes failed", LogLevel::ERR);
        CloseHandle(hSnapshot);
        return;
    }

    ULONGLONG sysTimeNow = FileTimeToULL(sysKernel) + FileTimeToULL(sysUser);

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    std::unordered_set<DWORD> alivePids;

    if (Process32First(hSnapshot, &pe))
    {
        do
        {
            ProcessInfo info;
            info.pid = pe.th32ProcessID;

            char name[MAX_PATH];
            size_t converted;
            wcstombs_s(&converted, name, pe.szExeFile, MAX_PATH);
            info.name = name;

            alivePids.insert(info.pid);

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, info.pid);
            if (hProcess == NULL)
            {
                info.accessDenied = true;
            }
            else
            {
                PROCESS_MEMORY_COUNTERS pmc;
                if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
                {
                    info.ramUsage = pmc.WorkingSetSize;
                }

                FILETIME ftCreate, ftExit, ftKernel, ftUser;
                if (GetProcessTimes(hProcess, &ftCreate, &ftExit, &ftKernel, &ftUser))
                {
                    ULONGLONG procTimeNow = FileTimeToULL(ftKernel) + FileTimeToULL(ftUser);

                    auto it = m_cpuHistory.find(info.pid);
                    if (it != m_cpuHistory.end())
                    {
                        ULONGLONG deltaProc = procTimeNow - it->second.lastProcTime;
                        ULONGLONG deltaSys = sysTimeNow - it->second.lastSysTime;

                        if (deltaSys > 0)
                        {
                            double cpu = (double)deltaProc / (double)deltaSys;
                            cpu = cpu * 100.0 * m_cpuCoreCount;
                            info.cpuUsage = cpu;
                        }

                        newCpuHistory[info.pid] = { procTimeNow, sysTimeNow };
                    }
                    else
                    {
                        newCpuHistory[info.pid] = { procTimeNow, sysTimeNow };
                        info.cpuUsage = 0.0;
                    }
                }

                CloseHandle(hProcess);
            }

            newProcessList.push_back(info);

        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);

    {
        std::lock_guard<std::mutex> lock(m_dataMutex);
        m_processList = std::move(newProcessList);
        m_cpuHistory = std::move(newCpuHistory);
    }

    Logger::GetInstance().Log(
        "ProcessMonitor updated. Total processes: " + std::to_string(m_processList.size()),
        LogLevel::DEBUG
    );
}

MetricType ProcessMonitor::GetMetricType() const
{
    return MetricType::PROCESS;
}

MonitorData ProcessMonitor::GetLastData() const
{
    std::lock_guard<std::mutex> lock(m_dataMutex);
    return m_processList;
}

ULONGLONG ProcessMonitor::FileTimeToULL(const FILETIME& ft) const
{
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart;
}
