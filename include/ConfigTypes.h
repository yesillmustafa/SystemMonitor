#pragma once
#include <string>
#include "LogLevel.h"

struct ProfilingConfig {
	bool enableProfiling = false;
};

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

struct ProcessConfig {
	int intervalSeconds = 2;
};

struct ApplicationConfig {
	int sleepMs = 100;
};

struct LoggerConfig {
	LogLevel minLevel = LogLevel::INFO;

	bool enableConsoleLog = true;
	bool enableFileLog = true;

	std::string filePath = "logs/system.log";
};