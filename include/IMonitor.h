#pragma once

#include "MetricType.h"

class IMonitor
{
public:
	virtual ~IMonitor() = default;

	virtual void Update() = 0;
	virtual MetricType GetMetricType() const = 0;
	virtual double GetLastValue() = 0;
};