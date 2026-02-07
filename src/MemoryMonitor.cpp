#include "MemoryMonitor.h"
#include "Logger.h"
#include "FormatUtils.h"

MemoryMonitor::MemoryMonitor(int intervalSeconds):
	m_intervalSeconds(intervalSeconds),
	m_lastRun(std::chrono::steady_clock::now())
{
	m_memStatus.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&m_memStatus);

}

MemoryMonitor::~MemoryMonitor()
{
	Stop();
}

void MemoryMonitor::Start()
{
	if (m_running)
		return;

	m_running = true;
	m_worker = std::thread(&MemoryMonitor::WorkerLoop, this);

	Logger::GetInstance().Log("RAM thread started", LogLevel::DEBUG);

}

void MemoryMonitor::Stop()
{
	if (!m_running)
		return;

	m_running = false;

	if (m_worker.joinable())
		m_worker.join();

	Logger::GetInstance().Log("RAM thread stopped", LogLevel::DEBUG);
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

MetricType MemoryMonitor::GetMetricType() const
{
	return MetricType::RAM;
}

double MemoryMonitor::GetLastValue() const
{
	return m_lastUsage;
}

void MemoryMonitor::WorkerLoop()
{
	while (m_running)
	{
		UpdateInternal();

		std::this_thread::sleep_for(std::chrono::seconds(m_intervalSeconds));
	}
}

void MemoryMonitor::UpdateInternal()
{
	double usage = GetUsagePercentage();
	{
		std::lock_guard<std::mutex> lock(m_dataMutex);
		m_lastUsage = usage;
	}
	Logger::GetInstance().Log(
		"RAM worker: " + FormatUtils::FormatPercent(usage) + "%",
		LogLevel::DEBUG
	);
}

