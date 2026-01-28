#pragma once

#include <Windows.h>
#include <IMonitor.h>
#include <chrono>


class MemoryMonitor : public IMonitor
{
public:

	explicit MemoryMonitor(int intervalSeconds);

	void Update() override;

private:

	double GetUsagePercantage(); // Toplam RAM'e göre kullaným yüzdesi

	int m_intervalSeconds;
	std::chrono::steady_clock::time_point m_lastRun;

	MEMORYSTATUSEX m_memStatus;

};