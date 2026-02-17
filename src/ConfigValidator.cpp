#include "ConfigValidator.h"

namespace Defaults
{
    // CPU
    constexpr int CPU_INTERVAL = 1;
    constexpr double CPU_WARNING = 70.0;
    constexpr double CPU_CRITICAL = 90.0;

    // RAM
    constexpr int RAM_INTERVAL = 3;
    constexpr double RAM_WARNING = 75.0;
    constexpr double RAM_CRITICAL = 90.0;

    // Process
    constexpr int PROCESS_INTERVAL = 2;

    // Application
    constexpr int APP_SLEEP = 100;

    // Limits
    constexpr int MAX_INTERVAL = 60;     // saniye
    constexpr int MAX_SLEEP = 10000;     // ms
}

void ConfigValidator::Validate(Config& config)
{
    // CPU
    if (config.m_cpu.intervalSeconds <= 0 || config.m_cpu.intervalSeconds > Defaults::MAX_INTERVAL)
        config.m_cpu.intervalSeconds = Defaults::CPU_INTERVAL;

    if (config.m_cpu.warningThreshold <= 0 || config.m_cpu.warningThreshold > 100)
        config.m_cpu.warningThreshold = Defaults::CPU_WARNING;

    if (config.m_cpu.criticalThreshold <= 0 || config.m_cpu.criticalThreshold > 100)
        config.m_cpu.criticalThreshold = Defaults::CPU_CRITICAL;

    if (config.m_cpu.warningThreshold >= config.m_cpu.criticalThreshold)
    {
        config.m_cpu.warningThreshold = Defaults::CPU_WARNING;
        config.m_cpu.criticalThreshold = Defaults::CPU_CRITICAL;
    }

    // RAM
    if (config.m_ram.intervalSeconds <= 0 || config.m_ram.intervalSeconds > Defaults::MAX_INTERVAL)
        config.m_ram.intervalSeconds = Defaults::RAM_INTERVAL;

    if (config.m_ram.warningThreshold <= 0 || config.m_ram.warningThreshold > 100)
        config.m_ram.warningThreshold = Defaults::RAM_WARNING;

    if (config.m_ram.criticalThreshold <= 0 || config.m_ram.criticalThreshold > 100)
        config.m_ram.criticalThreshold = Defaults::RAM_CRITICAL;

    if (config.m_ram.warningThreshold >= config.m_ram.criticalThreshold)
    {
        config.m_ram.warningThreshold = Defaults::RAM_WARNING;
        config.m_ram.criticalThreshold = Defaults::RAM_CRITICAL;
    }

    // Process
    if (config.m_process.intervalSeconds <= 0 || config.m_process.intervalSeconds > Defaults::MAX_INTERVAL)
        config.m_process.intervalSeconds = Defaults::PROCESS_INTERVAL;

    // Application
    if (config.m_app.sleepMs <= 0 || config.m_app.sleepMs > Defaults::MAX_SLEEP)
        config.m_app.sleepMs = Defaults::APP_SLEEP;

    // Logger
    if (config.m_logger.enableFileLog && config.m_logger.filePath.empty())
    {
        // File log açık ama path yok -> disable
        config.m_logger.enableFileLog = false;
    }
}