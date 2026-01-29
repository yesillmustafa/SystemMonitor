#pragma once

#include <string>
#include "Logger.h"

class Config
{
public:
	static Config& GetInstance();

	int GetCpuIntervalSeconds() const;
	int GetRamIntervalSeconds() const;

	LogLevel GetMinLogLevel() const;
	std::string GetLogFilePath() const;

	int GetTickSleepMillisecons() const;

private:
	Config();

	int m_CpuIntervalSeconds;
	int m_RamIntervalSeconds;

	LogLevel m_minLogLevel;
	std::string m_logFilePath;

	int m_tickSleepMs;

};
