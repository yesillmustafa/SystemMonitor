#include <iostream>
#include <Schedular.h>
#include <thread>

Schedular::Schedular(int intervalSeconds)
	: m_intervalSeconds(intervalSeconds),
	m_lastRun(std::chrono::steady_clock::now())
{
}

void Schedular::Tick()
{
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastRun);

	if (elapsed.count() >= m_intervalSeconds)
	{
		std::cout << "[Schedular] Tick\n";

		// burada ölçümler yapýlacak

		m_lastRun = now;
	}

	//CPU'yu %100 yememek için
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}