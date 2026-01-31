#pragma once
#include <string>
#include "LogLevel.h"

struct CpuConfig {
	int intervalSeconds = 1;
};

struct RamConfig {
	int intervalSeconds = 3;
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