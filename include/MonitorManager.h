#pragma once

#include "IMonitor.h"
#include <vector>
#include <memory>
//#include "AlertManager.h"

class MonitorManager
{
public:
	void RegisterMonitor(std::shared_ptr<IMonitor> monitor);

	void StartAll();
	void StopAll();

	std::vector<std::shared_ptr<IMonitor>> GetMonitors() const;

private:
	std::vector<std::shared_ptr<IMonitor>> m_monitors;

	// AlertManager m_alertManager;
};
