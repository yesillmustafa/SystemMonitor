#include "MonitorManager.h"

MonitorManager::~MonitorManager()
{
	StopAll();
}

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

void MonitorManager::SetAlertManager(std::shared_ptr<AlertManager> alertManager)
{
	m_alertManager = std::move(alertManager);
}

void MonitorManager::EvaluateAlerts()
{
	if (!m_alertManager) return;

	for (auto& m : m_monitors)
	{
		m_alertManager->Evaluate(m->GetMetricType(), m->GetLastData());
	}
}
