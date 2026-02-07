#include <iostream>
#include "Application.h"
#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"
#include "Config.h"
#include "Logger.h"


Application::Application()
	: m_running(true)
{
	auto& config = Config::GetInstance();


	m_monitorManager.RegisterMonitor(
		std::make_shared<CpuMonitor>(config.Cpu().intervalSeconds)
	);

	m_monitorManager.RegisterMonitor(
		std::make_shared<MemoryMonitor>(config.Ram().intervalSeconds)
	);

	m_monitorManager.RegisterMonitor(
		std::make_shared<ProcessMonitor>(config.Process().intervalSeconds)
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

	m_monitorManager.StartAll();

	while (m_running)
	{
		//CPU'yu %100 yememek icin
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	m_monitorManager.StopAll();

	Logger::GetInstance().Log("Application run loop exited", LogLevel::INFO);
}

void Application::RequestShutdown()
{
	Logger::GetInstance().Log("Shutdown requested", LogLevel::INFO);
	m_running = false;
}