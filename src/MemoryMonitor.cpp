#include <MemoryMonitor.h>
#include <Logger.h>
#include <iostream>

MemoryMonitor::MemoryMonitor(int intervalSeconds):
	m_intervalSeconds(intervalSeconds)
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

void MemoryMonitor::Update()
{
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastRun);

	if (elapsed.count() < m_intervalSeconds)
		return;

	double ram = GetUsagePercantage();

	Logger::GetInstance().Log("RAM: " + std::to_string(ram) + "%");
	std::cout << "RAM: " + std::to_string(ram) + "%" << std::endl;

	m_lastRun = now;
}