#pragma once

#include "Scheduler.h"
#include <atomic>

class Application
{
public:
	Application();
	~Application();

	void Run();
	void RequestShutdown();

private:
	Scheduler m_scheduler;
	std::atomic<bool> m_running;
};