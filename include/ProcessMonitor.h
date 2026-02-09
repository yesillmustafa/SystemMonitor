#pragma once
#include "IMonitor.h"
#include <Windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <atomic>

class ProcessMonitor : public IMonitor
{
public:

	explicit ProcessMonitor(int intervalSeconds);
	~ProcessMonitor();

	void Start() override;
	void Stop() override;
	MonitorData GetLastData() const override;
	MetricType GetMetricType() const override;

private:
	void WorkerLoop();
	void Update();

	ULONGLONG FileTimeToULL(const FILETIME& ft) const;

private:

	std::thread m_worker;
	std::atomic<bool> m_running{ false };
	mutable std::mutex m_dataMutex;

	std::condition_variable m_cv;
	std::mutex m_cvMutex;

	int m_intervalSeconds;
	std::vector<ProcessInfo> m_processList;

	struct CpuHistory
	{
		ULONGLONG lastProcTime = 0;  // kernel + user
		ULONGLONG lastSysTime = 0;  // system kernel + user
	};

	std::unordered_map<DWORD, CpuHistory> m_cpuHistory;
	int m_cpuCoreCount = 1;
};