#pragma once

#include <Windows.h>
#include <chrono>
#include "IMonitor.h"


class CpuMonitor : public IMonitor
{
public:
	explicit CpuMonitor(int intervalSeconds);
	
	void Update() override;
	std::string GetName() override;
	double GetLastValue() override;

private:

	bool ShouldRun();
	double GetUsage();

	ULONGLONG m_prevIdle;
	ULONGLONG m_prevKernel;
	ULONGLONG m_prevUser;

	int m_intervalSeconds;
	std::chrono::steady_clock::time_point m_lastRun;
	double m_lastUsage;

	ULONGLONG FileTimeToULL(const FILETIME& ft);

};