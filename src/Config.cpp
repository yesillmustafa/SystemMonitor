#include "Config.h"

Config::Config()
	: m_CpuIntervalSeconds(1),
	m_RamIntervalSeconds(3),
	m_minLogLevel(LogLevel::INFO),
	m_logFilePath("logs/system.log"),
	m_tickSleepMs(100)
{
}

Config& Config::GetInstance()
{
	static Config instance;
	return instance;
}

int Config::GetCpuIntervalSeconds() const
{
	return m_CpuIntervalSeconds;
}

int Config::GetRamIntervalSeconds() const
{
	return m_RamIntervalSeconds;
}

LogLevel Config::GetMinLogLevel() const
{
	return m_minLogLevel;
}

std::string Config::GetLogFilePath() const
{
	return m_logFilePath;
}

int Config::GetTickSleepMs() const
{
	return m_tickSleepMs;
}

