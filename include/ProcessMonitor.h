#pragma once

#include "IMonitor.h"
#include <vector>
#include <string>
#include <chrono>
#include <Windows.h>
#include <tlhelp32.h>
#include <unordered_map>

struct ProcessInfo {
	DWORD pid;
	std::string name;
	double cpuUsage = 0;
	SIZE_T ramUsage = 0;
	bool accessDenied = false;
};

struct CpuHistory
{
	ULONGLONG lastProcTime = 0;  // kernel + user
	ULONGLONG lastSysTime = 0;  // system kernel + user
};

class ProcessMonitor : public IMonitor
{
public:

	explicit ProcessMonitor(int intervalSeconds);
	~ProcessMonitor() = default;

	MetricType GetMetricType() const override;
	double GetLastValue() const override; // simdilik anlamsýz interface geregi var
	void Update() override;
	bool ShouldRun() override;

	const std::vector<ProcessInfo>& GetProcessList() const;

private:
	int m_intervalSeconds;
	std::chrono::steady_clock::time_point m_lastRun;

	std::vector<ProcessInfo> m_processList;

	double m_dummyValue = 0.0; // simdilik IMonitor'u tatmin etmek için

	std::unordered_map<DWORD, CpuHistory> m_cpuHistory;
	int m_cpuCoreCount = 1;

	ULONGLONG FileTimeToULL(const FILETIME& ft) const;

};