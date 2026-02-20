#include "ConfigLoader.h"
#include "Config.h"
#include "LogLevel.h"
#include "ConfigValidator.h"
#include "Logger.h"
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

static bool TryParseLogLevel(const std::string& value, LogLevel& out)
{
    std::string v = ToUpper(value);

    if (v == "DEBUG") { out = LogLevel::DEBUG; return true; }
    if (v == "INFO") { out = LogLevel::INFO; return true; }
    if (v == "WARNING") { out = LogLevel::WARNING; return true; }
    if (v == "ERROR") { out = LogLevel::ERR; return true; }

    return false;
}

bool TryParseInt(const std::string& s, int& out)
{
    try
    {
        size_t idx;
        int value = std::stoi(s, &idx);

        // "123abc" gibi durumlarý engelle
        if (idx != s.size())
            return false;

        out = value;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool TryParseDouble(const std::string& s, double& out)
{
    try
    {
        size_t idx;
        double value = std::stod(s, &idx);

        if (idx != s.size())
            return false;

        out = value;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

static bool TryParseBool(const std::string& value, bool& out)
{
    std::string v = ToUpper(value);

    if (v == "TRUE" || v == "1")
    {
        out = true;
        return true;
    }

    if (v == "FALSE" || v == "0")
    {
        out = false;
        return true;
    }

    return false;
}

static void AddParseError(
    std::vector<std::string>& errors,
    const std::string& section,
    const std::string& key,
    const std::string& value,
    const std::string& expected)
{
    errors.push_back(
        "[" + section + "] " + key +
        " has invalid value '" + value +
        "'. Expected: " + expected
    );
}

// ----------------------
// ConfigLoader
// ----------------------

bool ConfigLoader::LoadFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    // tempconfig
    Config tempConfig;

    std::vector<std::string> parseErrors;

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
        {
            parseErrors.push_back("Invalid line (missing '='): " + line);
            continue;
        }

        std::string key = ToUpper(Trim(line.substr(0, eqPos)));
        std::string value = Trim(line.substr(eqPos + 1));

        // ----------------------
        // CPU
        // ----------------------
        if (currentSection == "CPU")
        {
            if (key == "INTERVALSECONDS")
            {
                int v;
                if (TryParseInt(value, v))
                {
                    tempConfig.m_cpu.intervalSeconds = v;
                }
                else
                {
                    AddParseError(parseErrors, currentSection, key, value, "integer");
                }
            }
            else if (key == "WARNINGTHRESHOLD")
            {
                double v;
                if (TryParseDouble(value,v))
                {
                    tempConfig.m_cpu.warningThreshold = v;
                }
                else
                {
                    AddParseError(parseErrors, currentSection, key, value, "double");
                }
            }
            else if (key == "CRITICALTHRESHOLD")
            {
                double v;
                if(TryParseDouble(value,v))
                {
                    tempConfig.m_cpu.criticalThreshold = v;
                }
                else
                {
                    AddParseError(parseErrors, currentSection, key, value, "double");
                }
            }
        }
        // ----------------------
        // RAM
        // ----------------------
        else if (currentSection == "RAM")
        {
            if (key == "INTERVALSECONDS")
            {
                int v;
                if(TryParseInt(value,v))
                {
                    tempConfig.m_ram.intervalSeconds = v;
                }
                else
                {
                    AddParseError(parseErrors, currentSection, key, value, "integer");
                }
            }
            else if (key == "WARNINGTHRESHOLD")
            {
                double v;
                if (TryParseDouble(value,v))
                {
                    tempConfig.m_ram.warningThreshold = v;
                }
                else
                {
                    AddParseError(parseErrors, currentSection, key, value, "double");
                }
            }
            else if (key == "CRITICALTHRESHOLD")
            {
                double v;
                if(TryParseDouble(value,v))
                {
                    tempConfig.m_ram.criticalThreshold = v;
                }
                else
                {
                    AddParseError(parseErrors, currentSection, key, value, "double");
                }
            }
        }
        // ----------------------
        // ProcessMonitor
        // ----------------------
        else if (currentSection == "PROCESSMONITOR")
        {
            if (key == "INTERVALSECONDS")
            {
                int v;
                if(TryParseInt(value,v))
                {
                    tempConfig.m_process.intervalSeconds = v;
                }
                else
                {
                    AddParseError(parseErrors, currentSection, key, value, "integer");
                }
            }
        }
        // ----------------------
        // Application
        // ----------------------
        else if (currentSection == "APPLICATION")
        {
            if (key == "SLEEPMS")
            {
                int v;
                if(TryParseInt(value,v))
                {
                    tempConfig.m_app.sleepMs = v;
                }
                else
                {
                    AddParseError(parseErrors, currentSection, key, value, "integer");
                }
            }
        }
        // ----------------------
        // Logger
        // ----------------------
        else if (currentSection == "LOGGER")
        {
            if (key == "MINLEVEL")
            {
                LogLevel level;
                if (TryParseLogLevel(value, level))
                {
                    tempConfig.m_logger.minLevel = level;
                }
                else 
                {
                    AddParseError(parseErrors, currentSection, key, value, "DEBUG/INFO/WARNING/ERROR");
                }
            }
            else if (key == "ENABLECONSOLELOG")
            {
                bool v;
                if (TryParseBool(value, v))
                {
                    tempConfig.m_logger.enableConsoleLog = v;
                }
                else
                {
                    AddParseError(parseErrors, currentSection, key, value, "true/false");
                }
            }
            else if (key == "ENABLEFILELOG")
            {
                bool v;
                if (TryParseBool(value, v))
                {
                    tempConfig.m_logger.enableFileLog = v;
                }
                else
                {
                    AddParseError(parseErrors, currentSection, key, value, "true/false");
                }
            }
            else if (key == "LOGFILEPATH")
                tempConfig.m_logger.filePath = value;
        }
        // ----------------------
        // Profiling
        // ----------------------
        else if (currentSection == "PROFILING")
        {
            if (key == "ENABLEPROFILING")
            {
                bool v;
                if (TryParseBool(value, v))
                {
                    tempConfig.m_profiling.enableProfiling = v;
                }
                else
                {
                    AddParseError(parseErrors, currentSection, key, value, "true/false");
                }
            }
        }
    }

    auto errors = ConfigValidator::Validate(tempConfig);
    // parse hatalarýný ekle
    errors.insert(errors.end(), parseErrors.begin(), parseErrors.end());

    Config& config = Config::GetInstance();
    if (!errors.empty())
    {
        for (const auto& err : errors)
        {
            Logger::GetInstance().Log(err, LogLevel::ERR);
        }

        config.Apply(Config()); // DEFAULT RESET
        Logger::GetInstance().Log("Config validation failed. Using default config values.", LogLevel::WARNING);
    }
    else
    {
        config.Apply(tempConfig);
        Logger::GetInstance().Log("Config file loaded successfully.", LogLevel::INFO);
    }


    return true;
}
