#include <MemoryMonitor.h>

MemoryMonitor::MemoryMonitor()
{
	m_memStatus.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&m_memStatus);

}

double MemoryMonitor::GetUsagePercantage()
{
	// Güncel RAM durumunu al
	GlobalMemoryStatusEx(&m_memStatus);

	// Kullanýlan RAM
	DWORDLONG used = m_memStatus.ullTotalPhys - m_memStatus.ullAvailPhys;

	// YÜzdeyi hesapla
	return (double)used / m_memStatus.ullTotalPhys * 100.0;

}