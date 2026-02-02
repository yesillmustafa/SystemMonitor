#include "MemoryMonitor.h"
#include "Logger.h"
#include "FormatUtils.h"

MetricType MemoryMonitor::GetMetricType() const
{
	return MetricType::RAM;
}

double MemoryMonitor::GetLastValue()
{
	return m_lastUsage;
}

MemoryMonitor::MemoryMonitor(int intervalSeconds):
	m_intervalSeconds(intervalSeconds)
{
	m_memStatus.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&m_memStatus);

}

double MemoryMonitor::GetUsagePercentage()
{
	// Güncel RAM durumunu al
	GlobalMemoryStatusEx(&m_memStatus);

	// Kullanýlan RAM
	DWORDLONG used = m_memStatus.ullTotalPhys - m_memStatus.ullAvailPhys;

	// YÜzdeyi hesapla
	return (double)used / m_memStatus.ullTotalPhys * 100.0;

}

bool MemoryMonitor::ShouldRun()
{
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastRun);

	return elapsed.count() >= m_intervalSeconds;
}

void MemoryMonitor::Update()
{
	if (!ShouldRun())
		return;

	m_lastUsage = GetUsagePercentage();
	
	Logger::GetInstance().Log("RAM: " + FormatUtils::FormatPercent(m_lastUsage) + "%", LogLevel::DEBUG);

	m_lastRun = std::chrono::steady_clock::now();

}