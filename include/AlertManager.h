#pragma once

#include <string>
#include "MetricType.h"

enum class AlertState
{
    Normal,
    Warning,
    Critical
};

class AlertManager
{
public:
    void Evaluate(MetricType type, double value);

private:
    void CheckCpu(double value);
    void CheckRam(double value);

    double m_cpuWarningThreshold = 70.0;
    double m_cpuCriticalThreshold = 90.0;

    double m_ramWarningThreshold = 70.0;
    double m_ramCriticalThreshold = 90.0;
    
    AlertState m_cpuState = AlertState::Normal;
    AlertState m_ramState = AlertState::Normal;
};