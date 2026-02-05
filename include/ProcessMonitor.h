#pragma once

#include "IMonitor.h"
#include <vector>
#include <string>
#include <chrono>
#include <Windows.h>
#include <tlhelp32.h>
#include <unordered_set>

struct ProcessInfo {
	DWORD pid;
	std::string name;
	double cpuUsage = 0;
	SIZE_T ramUsage = 0;
};

class ProcessMonitor : public IMonitor
{
public:

	explicit ProcessMonitor(int intervalSeconds);
	~ProcessMonitor() = default;

	MetricType GetMetricType() const override;
	double GetLastValue() override; // simdilik anlamsýz interface geregi var
	void Update() override;

	const std::vector<ProcessInfo>& GetProcessList() const;

private:

	bool ShouldRun();
	
	int m_intervalSeconds;
	std::chrono::steady_clock::time_point m_lastRun;

	std::vector<ProcessInfo> m_processList;

	double m_dummyValue = 0.0; // simdilik IMonitor'u tatmin etmek için

	std::unordered_set<DWORD> m_failedOpenLogged;


};