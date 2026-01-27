#pragma once

#include <chrono>
#include <CpuMonitor.h>
#include <MemoryMonitor.h>


class Scheduler
{
public:
	explicit Scheduler(int intervalSeconds);
	void Tick();

private:
	CpuMonitor m_cpu;
	MemoryMonitor m_mem;

	int m_intervalSeconds;
	std::chrono::steady_clock::time_point m_lastRun;
};
