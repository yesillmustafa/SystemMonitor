#include "CpuMonitor.h"
#include "Logger.h"
#include "FormatUtils.h"
#include <chrono>

CpuMonitor::CpuMonitor(int intervalSeconds) :
	m_intervalSeconds(intervalSeconds)
{
	FILETIME idle, kernel, user;
	GetSystemTimes(&idle, &kernel, &user);

	m_prevIdle = FileTimeToULL(idle);
	m_prevKernel = FileTimeToULL(kernel);
	m_prevUser = FileTimeToULL(user);
}

CpuMonitor::~CpuMonitor()
{
	Stop();
}

void CpuMonitor::Start()
{
	if (m_running)
		return;

	m_running = true;
	m_worker = std::thread(&CpuMonitor::WorkerLoop, this);

	Logger::GetInstance().Log("CPU thread started", LogLevel::DEBUG);
}

void CpuMonitor::Stop()
{
	if (!m_running)
		return;

	m_running = false;
	m_cv.notify_all();

	if (m_worker.joinable())
		m_worker.join();

	Logger::GetInstance().Log("CPU thread stopped", LogLevel::DEBUG);
}

void CpuMonitor::WorkerLoop()
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

void CpuMonitor::Update()
{
	double usage = GetUsage();

	{
		std::lock_guard<std::mutex> lock(m_dataMutex);
		m_lastUsage = usage;
	}

	Logger::GetInstance().Log(
		"CPU: " + FormatUtils::FormatPercent(usage) + "%",
		LogLevel::DEBUG
	);
}

double CpuMonitor::GetUsage()
{
	FILETIME idle, kernel, user;
	GetSystemTimes(&idle, &kernel, &user);

	ULONGLONG idleTime = FileTimeToULL(idle);
	ULONGLONG kernelTime = FileTimeToULL(kernel);
	ULONGLONG userTime = FileTimeToULL(user);

	ULONGLONG idleDiff = idleTime - m_prevIdle;
	ULONGLONG kernelDiff = kernelTime - m_prevKernel;
	ULONGLONG userDiff = userTime - m_prevUser;

	ULONGLONG total = kernelDiff + userDiff;
	double usage = 0.0;

	if (total > 0)
		usage = (1.0 - (double)idleDiff / total) * 100.0;

	m_prevIdle = idleTime;
	m_prevKernel = kernelTime;
	m_prevUser = userTime;

	return usage;
}

MetricType CpuMonitor::GetMetricType() const
{
	return MetricType::CPU;
}

MonitorData CpuMonitor::GetLastData() const
{
	std::lock_guard<std::mutex> lock(m_dataMutex);
	return m_lastUsage;
}

ULONGLONG CpuMonitor::FileTimeToULL(const FILETIME& ft)
{
	return (static_cast<ULONGLONG>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}