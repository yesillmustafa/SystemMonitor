#pragma once
#include "MonitorManager.h"
#include "AlertManager.h"
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
	AlertManager m_alertManager;
	std::atomic<bool> m_running;
};