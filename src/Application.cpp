#include <iostream>
#include "Application.h"
#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "Config.h"
#include "Logger.h"


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

	Logger::GetInstance().Log("Application initialized", LogLevel::INFO);
}

Application::~Application()
{
	Logger::GetInstance().Log("Application shutting down", LogLevel::INFO);
}

void Application::Run()
{
	Logger::GetInstance().Log("Application is running", LogLevel::INFO);

	while (m_running)
	{
		m_scheduler.Tick();
	}

	Logger::GetInstance().Log("Application run loop exited", LogLevel::INFO);
}

void Application::RequestShutdown()
{
	Logger::GetInstance().Log("Shutdown requested", LogLevel::INFO);
	m_running = false;
}