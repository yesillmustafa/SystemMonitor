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
	const ProfilingConfig& Profiling() const;
	const ProcessConfig& Process() const;

private:
	Config() = default;

	CpuConfig m_cpu;
	RamConfig m_ram;
	SchedulerConfig m_scheduler;
	LoggerConfig m_logger;
	ProfilingConfig m_profiling;
	ProcessConfig m_process;

	friend class ConfigLoader;
};
