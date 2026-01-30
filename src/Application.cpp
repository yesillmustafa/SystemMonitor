#include <iostream>
#include "Application.h"
#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "Config.h"


Application::Application()
	: m_running(true)
{
	auto& config = Config::GetInstance();

	m_scheduler.RegisterMonitor(
		std::make_shared<CpuMonitor>(config.Cpu().intervalSeconds)
	);

	m_scheduler.RegisterMonitor(
		std::make_shared<MemoryMonitor>(config.Ram().intervalSeconds)
	); 

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
