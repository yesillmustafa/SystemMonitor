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
    AlertManager();
    void Evaluate(MetricType type, double value);

private:
    void CheckCpu(double value);
    void CheckRam(double value);

    double m_cpuWarningThreshold;
    double m_cpuCriticalThreshold;

    double m_ramWarningThreshold;
    double m_ramCriticalThreshold;
    
    AlertState m_cpuState = AlertState::Normal;
    AlertState m_ramState = AlertState::Normal;
};