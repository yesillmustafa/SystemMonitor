#include <iostream>
#include "Application.h"
#include "CpuMonitor.h"
#include "MemoryMonitor.h"


Application::Application()
	: m_running(true)
{
	m_scheduler.RegisterMonitor(std::make_shared<CpuMonitor>(1));   // 1 saniye
	m_scheduler.RegisterMonitor(std::make_shared<MemoryMonitor>(3)); // 3 saniye
	std::cout << "Application initialized\n";
}

Application::~Application()
{
	std::cout << "Application shutting down\n";
}

void Application::Run()
{
	std::cout << "Application is running..\n";

	while (m_running)
	{
		m_scheduler.Tick();
	}
}
