#include "ProcessMonitor.h"
#include "Logger.h"
#include "FormatUtils.h"
#include <Psapi.h>
#include <unordered_set>

ProcessMonitor::ProcessMonitor(int intervalSeconds)
	: m_intervalSeconds(intervalSeconds),
	m_lastRun(std::chrono::steady_clock::now())
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

    if (m_worker.joinable())
        m_worker.join();

    Logger::GetInstance().Log("ProcessMonitor thread stopped", LogLevel::DEBUG);
}

MetricType ProcessMonitor::GetMetricType() const
{
	return MetricType::PROCESS;
}

double ProcessMonitor::GetLastValue() const
{
	return m_dummyValue;
}

const std::vector<ProcessInfo>& ProcessMonitor::GetProcessList() const
{
    std::lock_guard<std::mutex> lock(m_dataMutex);
	return m_processList;
}

void ProcessMonitor::WorkerLoop()
{
    while (m_running)
    {
        UpdateInternal();

        std::this_thread::sleep_for(std::chrono::seconds(m_intervalSeconds));
    }
}

void ProcessMonitor::UpdateInternal()
{
    std::lock_guard<std::mutex> lock(m_dataMutex);

    m_processList.clear();

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        Logger::GetInstance().Log("Process snapshot could not be taken", LogLevel::ERR);
        return;
    }

    FILETIME sysIdle, sysKernel, sysUser;
    if (!GetSystemTimes(&sysIdle, &sysKernel, &sysUser)) {
        Logger::GetInstance().Log("GetSystemTimes failed", LogLevel::ERR);
        CloseHandle(hSnapshot);
        return;
    }

    ULONGLONG sysTimeNow = FileTimeToULL(sysKernel) + FileTimeToULL(sysUser);

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    std::unordered_set<DWORD> alivePids;

    if (Process32First(hSnapshot, &pe)) {
        do {
            ProcessInfo info;
            info.pid = pe.th32ProcessID;

            char name[MAX_PATH];
            size_t converted;
            wcstombs_s(&converted, name, pe.szExeFile, MAX_PATH);
            info.name = name;

            alivePids.insert(info.pid);

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, info.pid);
            if (hProcess == NULL) {
                info.accessDenied = true;
            }
            else {
                PROCESS_MEMORY_COUNTERS pmc;
                if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                    info.ramUsage = pmc.WorkingSetSize;
                }

                FILETIME ftCreate, ftExit, ftKernel, ftUser;
                if (GetProcessTimes(hProcess, &ftCreate, &ftExit, &ftKernel, &ftUser)) {
                    ULONGLONG procTimeNow = FileTimeToULL(ftKernel) + FileTimeToULL(ftUser);

                    auto it = m_cpuHistory.find(info.pid);
                    if (it != m_cpuHistory.end()) {
                        ULONGLONG deltaProc = procTimeNow - it->second.lastProcTime;
                        ULONGLONG deltaSys = sysTimeNow - it->second.lastSysTime;

                        if (deltaSys > 0) {
                            double cpu = (double)deltaProc / (double)deltaSys;
                            cpu = cpu * 100.0 * m_cpuCoreCount;
                            info.cpuUsage = cpu;
                        }
                        else {
                            info.cpuUsage = 0.0;
                        }

                        it->second.lastProcTime = procTimeNow;
                        it->second.lastSysTime = sysTimeNow;
                    }
                    else {
                        m_cpuHistory[info.pid] = { procTimeNow, sysTimeNow };
                        info.cpuUsage = 0.0;
                    }
                }

                CloseHandle(hProcess);
            }

            m_processList.push_back(info);

        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);

    for (auto it = m_cpuHistory.begin(); it != m_cpuHistory.end(); ) {
        if (alivePids.find(it->first) == alivePids.end()) {
            it = m_cpuHistory.erase(it);
        }
        else {
            ++it;
        }
    }

    Logger::GetInstance().Log(
        "ProcessMonitor updated. Total processes: " + std::to_string(m_processList.size()),
        LogLevel::DEBUG
    );
}

ULONGLONG ProcessMonitor::FileTimeToULL(const FILETIME& ft) const
{
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart;
}
