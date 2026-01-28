#pragma once

#include <Windows.h>
#include <IMonitor.h>
#include <chrono>

class CpuMonitor : public IMonitor
{
public:
	explicit CpuMonitor(int intervalSeconds);
	
	void Update() override;

private:

	double GetUsage();

	ULONGLONG m_prevIdle;
	ULONGLONG m_prevKernel;
	ULONGLONG m_prevUser;

	int m_intervalSeconds;
	std::chrono::steady_clock::time_point m_lastRun;

	ULONGLONG FileTimeToULL(const FILETIME& ft);

};