#include <iostream>
#include <Scheduler.h>
#include <thread>
#include <Logger.h>

Scheduler::Scheduler(int intervalSeconds)
	: m_intervalSeconds(intervalSeconds),
	m_lastRun(std::chrono::steady_clock::now())
{
}

void Scheduler::Tick()
{
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastRun);

	if (elapsed.count() >= m_intervalSeconds)
	{
		double cpu = m_cpu.GetUsage();
		double ram = m_mem.GetUsagePercantage();

		std::string msg = "CPU: " + std::to_string(cpu) + "%  RAM: " + std::to_string(ram) + "%";
		std::cout << "[SystemMonitor] CPU: " << cpu << "%  RAM: " << ram << "%\n";

		Logger::GetInstance().Log(msg);

		m_lastRun = now;
	}

	//CPU'yu %100 yememek için
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}