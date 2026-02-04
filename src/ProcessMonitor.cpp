#include "ProcessMonitor.h"
#include "Logger.h"
#include "FormatUtils.h"

ProcessMonitor::ProcessMonitor(int intervalSeconds)
	: m_intervalSeconds(intervalSeconds),
	m_lastRun(std::chrono::steady_clock::now())
{
	Logger::GetInstance().Log("ProcessMonitor initialized", LogLevel::DEBUG);
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
        Logger::GetInstance().Log("Process snapshot alýnamadý", LogLevel::ERR);
        return;
    }

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

            // CPU ve RAM ölçümü (þimdilik 0, sonraki adýmda ölçülecek)
            info.cpuUsage = 0.0;
            info.ramUsage = 0;

            m_processList.push_back(info);

        } while (Process32Next(hSnapshot, &pe));
    }
    else {
        Logger::GetInstance().Log("Process okunamadi", LogLevel::ERR);
    }

    CloseHandle(hSnapshot);

    Logger::GetInstance().Log("ProcessMonitor updated. Total processes: " +
        std::to_string(m_processList.size()), LogLevel::DEBUG);

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
