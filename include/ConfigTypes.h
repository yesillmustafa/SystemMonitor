#pragma once
#include <string>
#include "LogLevel.h"

struct CpuConfig {
	int intervalSeconds = 1;
	double warningThreshold = 70;
	double criticalThreshold = 90;
};

struct RamConfig {
	int intervalSeconds = 3;
	double warningThreshold = 75;
	double criticalThreshold = 90;
};

struct SchedulerConfig {
	int tickSleepMs = 100;
};

struct LoggerConfig {
	LogLevel minLevel = LogLevel::INFO;

	bool enableConsoleLog = true;
	bool enableFileLog = true;

	std::string filePath = "logs/system.log";
};