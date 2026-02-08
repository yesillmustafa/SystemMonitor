#include "ConfigLoader.h"
#include "Config.h"
#include "LogLevel.h"

#include <fstream>
#include <algorithm>
#include <cctype>

// ----------------------
// Helper functions
// ----------------------

static std::string Trim(const std::string& s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");

    if (start == std::string::npos)
        return "";

    return s.substr(start, end - start + 1);
}

static std::string ToUpper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::toupper(c); });
    return s;
}

static LogLevel ParseLogLevel(const std::string& value)
{
    std::string v = ToUpper(value);

    if (v == "DEBUG") return LogLevel::DEBUG;
    if (v == "INFO")  return LogLevel::INFO;
    if (v == "WARNING")  return LogLevel::WARNING;
    if (v == "ERROR") return LogLevel::ERR;

    return LogLevel::INFO; // fallback
}

// ----------------------
// ConfigLoader
// ----------------------

bool ConfigLoader::LoadFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    Config& config = Config::GetInstance();

    std::string line;
    std::string currentSection;

    while (std::getline(file, line))
    {
        line = Trim(line);

        // boþ satýr veya yorum
        if (line.empty() || line[0] == ';' || line[0] == '#')
            continue;

        // section [CPU], [Logger] ...
        if (line.front() == '[' && line.back() == ']')
        {
            currentSection = ToUpper(
                Trim(line.substr(1, line.size() - 2))
            );
            continue;
        }

        // key=value
        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos)
            continue;

        std::string key = ToUpper(Trim(line.substr(0, eqPos)));
        std::string value = Trim(line.substr(eqPos + 1));

        // ----------------------
        // CPU
        // ----------------------
        if (currentSection == "CPU")
        {
            if (key == "INTERVALSECONDS")
            {
                int v = std::stoi(value);
                if (v > 0)
                    config.m_cpu.intervalSeconds = v;
            }
            if (key == "WARNINGTHRESHOLD")
            {
                double v = std::stod(value);
                if (v > 0)
                    config.m_cpu.warningThreshold = v;
            }
            if (key == "CRITICALTHRESHOLD")
            {
                double v = std::stod(value);
                if (v > 0)
                    config.m_cpu.criticalThreshold = v;
            }
        }
        // ----------------------
        // RAM
        // ----------------------
        else if (currentSection == "RAM")
        {
            if (key == "INTERVALSECONDS")
            {
                int v = std::stoi(value);
                if (v > 0)
                    config.m_ram.intervalSeconds = v;
            }
            if (key == "WARNINGTHRESHOLD")
            {
                double v = std::stod(value);
                if (v > 0)
                    config.m_ram.warningThreshold = v;
            }
            if (key == "CRITICALTHRESHOLD")
            {
                double v = std::stod(value);
                if (v > 0)
                    config.m_ram.criticalThreshold = v;
            }
        }
        // ----------------------
        // ProcessMonitor
        // ----------------------
        else if (currentSection == "PROCESSMONITOR")
        {
            if (key == "INTERVALSECONDS")
            {
                int v = std::stoi(value);
                if (v > 0)
                    config.m_process.intervalSeconds = v;
            }
        }
        // ----------------------
        // Application
        // ----------------------
        else if (currentSection == "APPLICATION")
        {
            if (key == "SLEEPMS")
            {
                int v = std::stoi(value);
                if (v > 0)
                    config.m_app.sleepMs = v;
            }
        }
        // ----------------------
        // Logger
        // ----------------------
        else if (currentSection == "LOGGER")
        {
            if (key == "MINLEVEL")
                config.m_logger.minLevel = ParseLogLevel(value);
            else if (key == "ENABLECONSOLELOG")
                config.m_logger.enableConsoleLog = (ToUpper(value) == "TRUE");
            else if (key == "ENABLEFILELOG")
                config.m_logger.enableFileLog = (ToUpper(value) == "TRUE");
            else if (key == "LOGFILEPATH")
                config.m_logger.filePath = value;
        }
        // ----------------------
        // Profiling
        // ----------------------
        else if (currentSection == "PROFILING")
        {
            if (key == "ENABLEPROFILING")
                config.m_profiling.enableProfiling = (ToUpper(value) == "TRUE");
        }
    }

    return true;
}
