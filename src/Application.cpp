#include <iostream>
#include "Application.h"


Application::Application()
	: m_scheduler(2), // 2 saniyede bir
	m_running(true)
{
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
