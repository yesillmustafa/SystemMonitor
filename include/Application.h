#pragma once
#include "MonitorManager.h"
#include "AlertManager.h"
#include <mutex>
#include <condition_variable>
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

	std::condition_variable m_cv;
	std::mutex m_cvMutex;
};