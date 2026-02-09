#include "MemoryMonitor.h"
#include "Logger.h"
#include "FormatUtils.h"

MemoryMonitor::MemoryMonitor(int intervalSeconds):
	m_intervalSeconds(intervalSeconds)
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
	m_cv.notify_all();

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

void MemoryMonitor::WorkerLoop()
{
	std::unique_lock<std::mutex> lock(m_cvMutex);

	while (m_running)
	{
		Update();

		m_cv.wait_for(lock, std::chrono::seconds(m_intervalSeconds), [&]() {
			return !m_running;
		});
	}
}

void MemoryMonitor::Update()
{
	double usage = GetUsagePercentage();
	{
		std::lock_guard<std::mutex> lock(m_dataMutex);
		m_lastUsage = usage;
	}
	Logger::GetInstance().Log(
		"RAM: " + FormatUtils::FormatPercent(usage) + "%",
		LogLevel::DEBUG
	);
}

MetricType MemoryMonitor::GetMetricType() const
{
	return MetricType::RAM;
}

MonitorData MemoryMonitor::GetLastData() const
{
	std::lock_guard<std::mutex> lock(m_dataMutex);
	return m_lastUsage;
}


