#pragma once
#include <Windows.h>
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

	void Start() override;
	void Stop() override;
	MetricType GetMetricType() const override;
	MonitorData GetLastData() const override;

private:
	void WorkerLoop();
	void Update();

	double GetUsagePercentage(); // Toplam RAM'e göre kullaným yüzdesi

private:
	std::thread m_worker;
	std::atomic<bool> m_running{ false };
	mutable std::mutex m_dataMutex;

	int m_intervalSeconds;
	double m_lastUsage;

	MEMORYSTATUSEX m_memStatus;
};