#pragma once

#include <Windows.h>
#include "IMonitor.h"
#include "MetricType.h"
#include <thread>
#include <mutex>
#include <atomic>

class CpuMonitor : public IMonitor
{
public:
	explicit CpuMonitor(int intervalSeconds);
	~CpuMonitor();
	
	void Start() override;
	void Stop() override;
	MetricType GetMetricType() const override;
	MonitorData GetLastData() const override;

private:
	void WorkerLoop();
	void Update();

	double GetUsage();
	ULONGLONG FileTimeToULL(const FILETIME& ft);

private:
	std::thread m_worker;
	std::atomic<bool> m_running{ false };
	mutable std::mutex m_dataMutex;

	ULONGLONG m_prevIdle;
	ULONGLONG m_prevKernel;
	ULONGLONG m_prevUser;

	int m_intervalSeconds;
	double m_lastUsage;
};