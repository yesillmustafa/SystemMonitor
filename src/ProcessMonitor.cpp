#include "ProcessMonitor.h"
#include "Logger.h"
#include "FormatUtils.h"
#include <Psapi.h>

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

MetricType ProcessMonitor::GetMetricType() const
{
	return MetricType::PROCESS;
}

double ProcessMonitor::GetLastValue()
{
	return m_dummyValue;
}

void ProcessMonitor::Update()
{
    if (!ShouldRun())
        return;

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

    if (Process32First(hSnapshot, &pe)) {
        do {
            ProcessInfo info;
            info.pid = pe.th32ProcessID;

            char name[MAX_PATH];
            size_t converted;
            wcstombs_s(&converted, name, pe.szExeFile, MAX_PATH);
            info.name = name;

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, info.pid);
            if (hProcess != NULL) {

                // === RAM ===
                PROCESS_MEMORY_COUNTERS pmc;
                if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                    info.ramUsage = pmc.WorkingSetSize;
                }

                // === CPU ===
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
                        // Ýlk kez görülen process
                        m_cpuHistory[info.pid] = { procTimeNow, sysTimeNow };
                        info.cpuUsage = 0.0;
                    }
                }

                CloseHandle(hProcess);
            }
            else {
                if (m_failedOpenLogged.insert(info.pid).second) {
                    Logger::GetInstance().Log(
                        "OpenProcess failed for PID: " + std::to_string(info.pid),
                        LogLevel::DEBUG
                    );
                }
            }

            m_processList.push_back(info);

        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);

    Logger::GetInstance().Log(
        "ProcessMonitor updated. Total processes: " + std::to_string(m_processList.size()),
        LogLevel::DEBUG
    );

    m_lastRun = std::chrono::steady_clock::now();
}

const std::vector<ProcessInfo>& ProcessMonitor::GetProcessList() const
{
	return m_processList;
}

bool ProcessMonitor::ShouldRun()
{
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastRun);
	return elapsed.count() >= m_intervalSeconds;
}

ULONGLONG ProcessMonitor::FileTimeToULL(const FILETIME& ft) const
{
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart;
}
