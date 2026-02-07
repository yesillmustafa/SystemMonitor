#pragma once

#include <Windows.h>
#include <chrono>
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
	
	//void Update() override;
	//bool ShouldRun() override;
	MetricType GetMetricType() const override;
	double GetLastValue() const override;

	void Start();
	void Stop();

private:
	void WorkerLoop();
	void UpdateInternal();

private:
	std::thread m_worker;
	std::atomic<bool> m_running{ false };
	mutable std::mutex m_dataMutex;

	double GetUsage();

	ULONGLONG m_prevIdle;
	ULONGLONG m_prevKernel;
	ULONGLONG m_prevUser;

	int m_intervalSeconds;
	std::chrono::steady_clock::time_point m_lastRun;
	double m_lastUsage;

	ULONGLONG FileTimeToULL(const FILETIME& ft);

};