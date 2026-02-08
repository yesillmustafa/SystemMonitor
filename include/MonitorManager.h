#pragma once

#include "IMonitor.h"
#include <vector>
#include <memory>
#include "AlertManager.h"

class MonitorManager
{
public:
	void RegisterMonitor(std::shared_ptr<IMonitor> monitor);

	void StartAll();
	void StopAll();

	std::vector<std::shared_ptr<IMonitor>> GetMonitors() const;

	void SetAlertManager(std::shared_ptr<AlertManager> alertManager);
	void EvaluateAlerts();

private:
	std::vector<std::shared_ptr<IMonitor>> m_monitors;

	std::shared_ptr<AlertManager> m_alertManager;

};
