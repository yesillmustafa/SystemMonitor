#include "Config.h"

Config& Config::GetInstance()
{
	static Config instance;
	return instance;
}

const CpuConfig& Config::Cpu() const {
	return m_cpu;
}

const RamConfig& Config::Ram() const {
	return m_ram;
}

const SchedulerConfig& Config::Scheduler() const {
	return m_scheduler;
}

const LoggerConfig& Config::Logger() const {
	return m_logger;
}

const ProfilingConfig& Config::Profiling() const
{
	return m_profiling;
}

const ProcessConfig& Config::Process() const
{
	return m_process;
}
