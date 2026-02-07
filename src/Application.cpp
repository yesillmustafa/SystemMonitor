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

	auto cpuMon = std::make_shared<CpuMonitor>(config.Cpu().intervalSeconds);
	cpuMon->Start();
	m_scheduler.RegisterMonitor(cpuMon);
	/*m_scheduler.RegisterMonitor(
		std::make_shared<CpuMonitor>(config.Cpu().intervalSeconds)
	);*/

	auto ramMon = std::make_shared<MemoryMonitor>(config.Ram().intervalSeconds);
	ramMon->Start();
	m_scheduler.RegisterMonitor(ramMon);
	//m_scheduler.RegisterMonitor(
	//	std::make_shared<MemoryMonitor>(config.Ram().intervalSeconds)
	//);

	auto procMon = std::make_shared<ProcessMonitor>(config.Process().intervalSeconds);
	procMon->Start();
	m_scheduler.RegisterMonitor(procMon);
	/*m_scheduler.RegisterMonitor(
		std::make_shared<ProcessMonitor>(config.Process().intervalSeconds)
	);*/

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