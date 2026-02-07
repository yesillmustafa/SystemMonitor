#pragma once
#include "MonitorManager.h"
#include <atomic>

class Application
{
public:
	Application();
	~Application();

	void Run();
	void RequestShutdown();

private:
	MonitorManager m_monitorManager;
	std::atomic<bool> m_running;
};