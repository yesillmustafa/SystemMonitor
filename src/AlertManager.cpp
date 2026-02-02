#include "AlertManager.h"
#include "Logger.h"

void AlertManager::Evaluate(MetricType type, double value)
{
    switch (type)
    {
    case MetricType::CPU:
        CheckCpu(value);
        break;
    case MetricType::RAM:
        CheckRam(value);
        break;
    default:
        break;
    }
}

void AlertManager::CheckCpu(double value)
{
    AlertState newState = AlertState::Normal;

    if (value >= m_cpuCriticalThreshold)
        newState = AlertState::Critical;
    else if (value >= m_cpuWarningThreshold)
        newState = AlertState::Warning;

    if (newState == m_cpuState)
        return; // state degismedi -> log atma

    // State degisti -> log at
    switch (newState)
    {
    case AlertState::Critical:
        Logger::GetInstance().Log("CPU CRITICAL: " + std::to_string(value) + "%", LogLevel::ERR);
        break;
    case AlertState::Warning:
        Logger::GetInstance().Log("CPU HIGH: " + std::to_string(value) + "%", LogLevel::WARNING);
        break;
    case AlertState::Normal:
        Logger::GetInstance().Log("CPU back to normal: " + std::to_string(value) + "%", LogLevel::INFO);
        break;
    }

    m_cpuState = newState;
}

void AlertManager::CheckRam(double value)
{
    AlertState newState = AlertState::Normal;

    if (value >= m_ramCriticalThreshold)
        newState = AlertState::Critical;
    else if (value >= m_ramWarningThreshold)
        newState = AlertState::Warning;

    if (newState == m_ramState)
        return;

    switch (newState)
    {
    case AlertState::Critical:
        Logger::GetInstance().Log("RAM CRITICAL: " + std::to_string(value) + "%", LogLevel::ERR);
        break;
    case AlertState::Warning:
        Logger::GetInstance().Log("RAM HIGH: " + std::to_string(value) + "%", LogLevel::WARNING);
        break;
    case AlertState::Normal:
        Logger::GetInstance().Log("RAM back to normal: " + std::to_string(value) + "%", LogLevel::INFO);
        break;
    }

    m_ramState = newState;
}