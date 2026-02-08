#include "AlertManager.h"
#include "Logger.h"
#include "Config.h"
#include "FormatUtils.h"

AlertManager::AlertManager()
{
    const auto& cfg = Config::GetInstance();

    m_cpuWarningThreshold = cfg.Cpu().warningThreshold;
    m_cpuCriticalThreshold = cfg.Cpu().criticalThreshold;

    m_ramWarningThreshold = cfg.Ram().warningThreshold;
    m_ramCriticalThreshold = cfg.Ram().criticalThreshold;
}

void AlertManager::Evaluate(MetricType type, const MonitorData& data)
{
    switch (type)
    {
    case MetricType::CPU:
        if (auto value = std::get_if<double>(&data))
            CheckMetric("CPU", *value, m_cpuWarningThreshold, m_cpuCriticalThreshold, m_cpuState);
        break;
    case MetricType::RAM:
        if (auto value = std::get_if<double>(&data))
         CheckMetric("RAM", *value, m_ramWarningThreshold, m_ramCriticalThreshold, m_ramState);
        break;
    default:
        break;
    }
}

void AlertManager::CheckMetric(const std::string& name, double value,
    double warningThreshold, double criticalThreshold,
    AlertState& currentState)
{
    AlertState newState = AlertState::Normal;

    if (value >= criticalThreshold)
        newState = AlertState::Critical;
    else if (value >= warningThreshold)
        newState = AlertState::Warning;

    if (newState == currentState)
        return; // state degismedi -> log atma

    // State degisti -> log at
    switch (newState)
    {
    case AlertState::Critical:
        Logger::GetInstance().Log(name + " CRITICAL: " + FormatUtils::FormatPercent(value) + "%", LogLevel::ERR);
        break;
    case AlertState::Warning:
        Logger::GetInstance().Log(name + " HIGH: " + FormatUtils::FormatPercent(value) + "%", LogLevel::WARNING);
        break;
    case AlertState::Normal:
        Logger::GetInstance().Log(name + " back to normal: " + FormatUtils::FormatPercent(value) + "%", LogLevel::INFO);
        break;
    }

    currentState = newState;
}
