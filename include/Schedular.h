#pragma once

#include <chrono>

class Schedular
{
public:
	explicit Schedular(int intervalSeconds);
	void Tick();

private:
	int m_intervalSeconds;
	std::chrono::steady_clock::time_point m_lastRun;
};
