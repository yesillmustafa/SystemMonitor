#include <thread>
#include "Scheduler.h"
#include "Config.h"
#include <chrono>
#include "Logger.h"

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
    /*auto start = std::chrono::steady_clock::now();

    for (auto& monitor : m_monitors)
    {
        if (!monitor->ShouldRun())
            continue;

        if (Config::GetInstance().Profiling().enableProfiling) {
            auto start = std::chrono::steady_clock::now();

            monitor->Update();

            auto end = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            Logger::GetInstance().Log(
                "Update duration: " + std::to_string(elapsed.count()) + " us",
                LogLevel::DEBUG
            );
        }
        else {
            monitor->Update();
        }
        m_alertManager.Evaluate(monitor->GetMetricType(), monitor->GetLastValue());
    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    Logger::GetInstance().Log(
        "Tick duration: " + std::to_string(elapsed.count()) + " us",
        LogLevel::DEBUG);*/

	//CPU'yu %100 yememek icin
	std::this_thread::sleep_for(std::chrono::milliseconds(m_tickMs));
}