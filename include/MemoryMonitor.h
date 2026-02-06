#pragma once

#include <Windows.h>
#include <chrono>
#include "IMonitor.h"
#include "MetricType.h"

class MemoryMonitor : public IMonitor
{
public:

	explicit MemoryMonitor(int intervalSeconds);

	void Update() override;
	bool ShouldRun() override;
	MetricType GetMetricType() const override;
	double GetLastValue() const override;

private:
	double GetUsagePercentage(); // Toplam RAM'e göre kullaným yüzdesi

	int m_intervalSeconds;
	std::chrono::steady_clock::time_point m_lastRun;
	double m_lastUsage;

	MEMORYSTATUSEX m_memStatus;

};