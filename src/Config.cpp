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

const ApplicationConfig& Config::App() const {
	return m_app;
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

void Config::Apply(const Config& other)
{
	m_cpu = other.m_cpu;
	m_ram = other.m_ram;
	m_app = other.m_app;
	m_logger = other.m_logger;
	m_profiling = other.m_profiling;
	m_process = other.m_process;
}
