#pragma once

#include <Windows.h>
#include <chrono>
#include "IMonitor.h"
#include "MetricType.h"
#include <thread>
#include <mutex>
#include <atomic>

class MemoryMonitor : public IMonitor
{
public:

	explicit MemoryMonitor(int intervalSeconds);
	~MemoryMonitor();

	void Start();
	void Stop();

	/*void Update() override;
	bool ShouldRun() override;*/
	MetricType GetMetricType() const override;
	double GetLastValue() const override;

private:
	void WorkerLoop();
	void UpdateInternal();

private:
	std::thread m_worker;
	std::atomic<bool> m_running{ false };
	mutable std::mutex m_dataMutex;

	double GetUsagePercentage(); // Toplam RAM'e göre kullaným yüzdesi

	int m_intervalSeconds;
	std::chrono::steady_clock::time_point m_lastRun;
	double m_lastUsage;

	MEMORYSTATUSEX m_memStatus;

};