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

	m_monitorManager.SetAlertManager(std::make_shared<AlertManager>(m_alertManager));

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
	std::unique_lock<std::mutex> lock(m_cvMutex);

	while (m_running)
	{
		m_monitorManager.EvaluateAlerts();

		//CPU'yu %100 yememek icin
		m_cv.wait_for(lock,
			std::chrono::milliseconds(Config::GetInstance().App().sleepMs),
			[&]() { return !m_running; }
		);
	}

	m_monitorManager.StopAll();

	Logger::GetInstance().Log("Application run loop exited", LogLevel::INFO);
	Logger::GetInstance().Shutdown();
}

void Application::RequestShutdown()
{
	Logger::GetInstance().Log("Shutdown requested", LogLevel::INFO);
	m_running = false;
	m_cv.notify_all();
}