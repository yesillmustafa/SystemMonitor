#pragma once

#include "IMonitor.h"
#include <chrono>

class ProcessMonitor : public IMonitor
{
public:

	explicit ProcessMonitor(int intervalSeconds);

	MetricType GetMetricType() const override;
	double GetLastValue() override; // simdilik anlamsýz interface geregi var
	void Update() override;

private:

	bool ShouldRun();
	
	int m_intervalSeconds;
	std::chrono::steady_clock::time_point m_lastRun;

	double m_dummyValue = 0.0; // simdilik IMonitor'u tatmin etmek için


};