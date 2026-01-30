#pragma once

#include "ConfigTypes.h"

class Config
{
public:
	static Config& GetInstance();
	
	const CpuConfig& Cpu() const;
	const RamConfig& Ram() const;
	const SchedulerConfig& Scheduler() const;
	const LoggerConfig& Logger() const;

private:
	Config() = default;

	CpuConfig m_cpu;
	RamConfig m_ram;
	SchedulerConfig m_scheduler;
	LoggerConfig m_logger;

	//friend class ConfigLoader;
};
