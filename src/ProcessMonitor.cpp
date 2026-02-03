#include "ProcessMonitor.h"
#include "Logger.h"

ProcessMonitor::ProcessMonitor(int intervalSeconds)
	: m_intervalSeconds(intervalSeconds),
	m_lastRun(std::chrono::steady_clock::now())
{
	Logger::GetInstance().Log("ProcessMonitor initialized", LogLevel::DEBUG);
}

MetricType ProcessMonitor::GetMetricType() const
{
	return MetricType::PROCESS;
}

double ProcessMonitor::GetLastValue()
{
	return m_dummyValue;
}

void ProcessMonitor::Update()
{
	if (!ShouldRun())
		return;

	Logger::GetInstance().Log("ProcessMonitor tick", LogLevel::DEBUG);

	m_lastRun = std::chrono::steady_clock::now();
}

bool ProcessMonitor::ShouldRun()
{
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastRun);
	return elapsed.count() >= m_intervalSeconds;
}

