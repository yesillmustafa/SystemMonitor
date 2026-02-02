#include "AlertManager.h"
#include "Logger.h"

void AlertManager::Evaluate(const std::string& name, double value)
{
	if (name == "CPU")
		CheckCpu(value);
	else if (name == "RAM")
		CheckRam(value);
}
void AlertManager::CheckCpu(double value)
{
	if (value >= m_cpuCriticalThreshold && m_cpuState != AlertState::Critical)
	{
		Logger::GetInstance().Log("CPU CRITICAL: " + std::to_string(value) + "%", LogLevel::ERR);
		m_cpuState = AlertState::Critical;
	}
	else if (value >= m_cpuWarningThreshold && m_cpuState != AlertState::Warning)
	{
		Logger::GetInstance().Log("CPU HIGH: " + std::to_string(value) + "%", LogLevel::WARNING);
		m_cpuState = AlertState::Warning;
	}
	else if (value < m_cpuWarningThreshold && m_cpuState != AlertState::Normal)
	{
		Logger::GetInstance().Log("CPU back to normal: " + std::to_string(value) + "%", LogLevel::INFO);
		m_cpuState = AlertState::Normal;
	}
}

void AlertManager::CheckRam(double value)
{
	if (value >= m_ramCriticalThreshold && m_ramState != AlertState::Critical)
	{
		Logger::GetInstance().Log("RAM CRITICAL: " + std::to_string(value) + "%", LogLevel::ERR);
		m_ramState = AlertState::Critical;
	}
	else if (value >= m_ramWarningThreshold && m_ramState != AlertState::Warning)
	{
		Logger::GetInstance().Log("RAM HIGH: " + std::to_string(value) + "%", LogLevel::WARNING);
		m_ramState = AlertState::Warning;
	}
	else if (value < m_ramWarningThreshold && m_ramState != AlertState::Normal)
	{
		Logger::GetInstance().Log("RAM back to normal: " + std::to_string(value) + "%", LogLevel::INFO);
		m_ramState = AlertState::Normal;
	}
}
