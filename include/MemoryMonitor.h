#pragma once

#include <Windows.h>

class MemoryMonitor
{
public:

	MemoryMonitor();

	double GetUsagePercantage(); // Toplam RAM'e göre kullaným yüzdesi

private:

	MEMORYSTATUSEX m_memStatus;

};