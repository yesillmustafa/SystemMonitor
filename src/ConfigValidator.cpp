#include "ConfigValidator.h"

namespace Defaults
{
    constexpr int CPU_INTERVAL = 1;
    constexpr double CPU_WARNING = 70.0;
    constexpr double CPU_CRITICAL = 90.0;

    constexpr int RAM_INTERVAL = 3;
    constexpr double RAM_WARNING = 75.0;
    constexpr double RAM_CRITICAL = 90.0;

    constexpr int PROCESS_INTERVAL = 2;
    constexpr int APP_SLEEP = 100;

    constexpr int MAX_INTERVAL = 60;
    constexpr int MAX_SLEEP = 10000;
}

std::vector<std::string> ConfigValidator::Validate(const Config& config)
{
    std::vector<std::string> errors;

    // CPU
    if (config.m_cpu.intervalSeconds <= 0 || config.m_cpu.intervalSeconds > Defaults::MAX_INTERVAL)
    {
        errors.push_back("CPU interval must be between 0-60 seconds");
    }

    if (config.m_cpu.warningThreshold <= 0 || config.m_cpu.warningThreshold > 100)
    {
        errors.push_back("CPU warning threshold must be between 0-100");
    }

    if (config.m_cpu.criticalThreshold <= 0 || config.m_cpu.criticalThreshold > 100)
    {
        errors.push_back("CPU critical threshold must be between 0-100");
    }

    if (config.m_cpu.warningThreshold >= config.m_cpu.criticalThreshold)
    {
        errors.push_back("CPU warning threshold must be less than critical");
    }

    // RAM
    if (config.m_ram.intervalSeconds <= 0 || config.m_ram.intervalSeconds > Defaults::MAX_INTERVAL)
    {
        errors.push_back("RAM interval must be between 0-60 seconds");
    }

    if (config.m_ram.warningThreshold <= 0 || config.m_ram.warningThreshold > 100)
    {
        errors.push_back("RAM warning threshold must be between 0-100");
    }

    if (config.m_ram.criticalThreshold <= 0 || config.m_ram.criticalThreshold > 100)
    {
        errors.push_back("RAM critical threshold must be between 0-100");
    }

    if (config.m_ram.warningThreshold >= config.m_ram.criticalThreshold)
    {
        errors.push_back("RAM warning threshold must be less than critical");
    }

    // Process
    if (config.m_process.intervalSeconds <= 0 || config.m_process.intervalSeconds > Defaults::MAX_INTERVAL)
    {
        errors.push_back("Process monitor interval must be between 0-60 seconds");
    }

    // Application
    if (config.m_app.sleepMs <= 0 || config.m_app.sleepMs > Defaults::MAX_SLEEP)
    {
        errors.push_back("Application sleep must be between 0-10000 ms");
    }

    // Logger
    if (config.m_logger.enableFileLog && config.m_logger.filePath.empty())
    {
        errors.push_back("File logging enabled but filePath is empty");
    }

    return errors;
}