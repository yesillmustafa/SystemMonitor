#pragma once

#include "MetricType.h"

class IMonitor
{
public:
	virtual ~IMonitor() = default;

	virtual void Update() = 0;
	virtual bool ShouldRun() = 0;
	virtual MetricType GetMetricType() const = 0;
	virtual double GetLastValue() const = 0;
};