#include <iostream>
#include <thread>
#include "Scheduler.h"
#include "Config.h"

Scheduler::Scheduler()
    : m_tickMs(Config::GetInstance().Scheduler().tickSleepMs)
{
}

void Scheduler::RegisterMonitor(std::shared_ptr<IMonitor> monitor)
{
	m_monitors.push_back(monitor);
}

void Scheduler::Tick()
{
	for (auto& monitor : m_monitors)
	{
		monitor->Update();
	}

	//CPU'yu %100 yememek için
	std::this_thread::sleep_for(std::chrono::milliseconds(m_tickMs));
}