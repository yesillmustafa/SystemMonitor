#pragma once

#include <Windows.h>
#include <chrono>
#include "IMonitor.h"


class MemoryMonitor : public IMonitor
{
public:

	explicit MemoryMonitor(int intervalSeconds);

	void Update() override;

private:

	bool ShouldRun();
	double GetUsagePercantage(); // Toplam RAM'e göre kullaným yüzdesi

	int m_intervalSeconds;
	std::chrono::steady_clock::time_point m_lastRun;

	MEMORYSTATUSEX m_memStatus;

};