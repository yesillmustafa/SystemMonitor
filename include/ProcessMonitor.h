#pragma once

#include "IMonitor.h"
#include <vector>
#include <string>
#include <chrono>
#include <Windows.h>
#include <tlhelp32.h>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <atomic>

struct ProcessInfo {
	DWORD pid;
	std::string name;
	double cpuUsage = 0;
	SIZE_T ramUsage = 0;
	bool accessDenied = false;
};

class ProcessMonitor : public IMonitor
{
public:

	explicit ProcessMonitor(int intervalSeconds);
	~ProcessMonitor();

	void Start();
	void Stop();

	void Update() override; // Scheduler artýk bunu çaðýrmayacak interface geregi duruyor
	double GetLastValue() const override; // simdilik anlamsýz interface geregi var
	MetricType GetMetricType() const override;
	bool ShouldRun() override;

	const std::vector<ProcessInfo>& GetProcessList() const;

private:
	void WorkerLoop();      // thread fonksiyonu
	void UpdateInternal();  // eski Update içeriði buraya taþýnacak

private:

	std::thread m_worker;
	std::atomic<bool> m_running{ false };
	mutable std::mutex m_dataMutex;


	int m_intervalSeconds;
	std::chrono::steady_clock::time_point m_lastRun;

	std::vector<ProcessInfo> m_processList;

	double m_dummyValue = 0.0; // simdilik IMonitor'u tatmin etmek için

	struct CpuHistory
	{
		ULONGLONG lastProcTime = 0;  // kernel + user
		ULONGLONG lastSysTime = 0;  // system kernel + user
	};

	std::unordered_map<DWORD, CpuHistory> m_cpuHistory;
	int m_cpuCoreCount = 1;

	ULONGLONG FileTimeToULL(const FILETIME& ft) const;

};