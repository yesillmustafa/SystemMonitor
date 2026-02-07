#include "MonitorManager.h"

void MonitorManager::RegisterMonitor(std::shared_ptr<IMonitor> monitor)
{
	m_monitors.push_back(monitor);
}

void MonitorManager::StartAll()
{
	for (auto& m : m_monitors)
		m->Start();
}

void MonitorManager::StopAll()
{
	for (auto& m : m_monitors)
		m->Stop();
}

std::vector<std::shared_ptr<IMonitor>> MonitorManager::GetMonitors() const
{
	return m_monitors;
}
