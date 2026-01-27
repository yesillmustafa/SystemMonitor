#pragma once

#include <Scheduler.h>

class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	Scheduler m_scheduler;
	bool m_running;
};