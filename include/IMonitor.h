#pragma once
#include "MetricType.h"
#include <variant>
#include <vector>
#include "ProcessInfo.h"

using MonitorData = std::variant<double, std::vector<ProcessInfo>>;

class IMonitor
{
public:
	virtual ~IMonitor() = default;

	virtual void Start() = 0;
	virtual void Stop() = 0;

	// Monitor tipi (CPU,RAM,PROCESS)
	virtual MetricType GetMetricType() const = 0;
	// MonitorData ile her monitor kendi verisini döndürür.
	virtual MonitorData GetLastData() const = 0;
};