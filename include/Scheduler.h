#pragma once

#include <IMonitor.h>
#include <vector>
#include <memory>

class Scheduler
{
public:
	
	void RegisterMonitor(std::shared_ptr<IMonitor> monitor);
	void Tick();

private:
	std::vector<std::shared_ptr<IMonitor>> m_monitors;
};
