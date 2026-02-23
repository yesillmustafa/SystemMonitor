#include "ConfigLoader.h"
#include "Config.h"
#include "ConfigError.h"
#include "LogLevel.h"
#include "ConfigValidator.h"
#include "Logger.h"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <unordered_set>

// ----------------------
// Config Contract
// ----------------------

static const std::unordered_map<std::string, std::unordered_set<std::string>> kConfigSchema =
{
    { "CPU", { "INTERVALSECONDS", "WARNINGTHRESHOLD", "CRITICALTHRESHOLD" } },
    { "RAM", { "INTERVALSECONDS", "WARNINGTHRESHOLD", "CRITICALTHRESHOLD" } },
    { "PROCESSMONITOR", { "INTERVALSECONDS" } },
    { "APPLICATION", { "SLEEPMS" } },
    { "LOGGER", { "MINLEVEL", "ENABLECONSOLELOG", "ENABLEFILELOG", "LOGFILEPATH" } },
    { "PROFILING", { "ENABLEPROFILING" } }
};

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

static bool TryParseInt(const std::string& s, int& out)
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

static bool TryParseDouble(const std::string& s, double& out)
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

    bool isValidSection = true;

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

            if (kConfigSchema.find(currentSection) == kConfigSchema.end())
            {
                parseErrors.push_back(ConfigError::UnknownSection(currentSection));
                isValidSection = false;
            }
            else
            {
                isValidSection = true;
            }

            continue;
        }

        // key=value
        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos)
        {
            parseErrors.push_back(ConfigError::InvalidLine(line));
            continue;
        }

        std::string key = ToUpper(Trim(line.substr(0, eqPos)));
        std::string value = Trim(line.substr(eqPos + 1));

        // section yoksa
        if (currentSection.empty())
        {
            parseErrors.push_back(ConfigError::MissingSection(line));
            continue;
        }

        // unknown section ise ignore et
        if (!isValidSection)
        {
            continue;
        }

        // allowed key kontrolü
        auto it = kConfigSchema.find(currentSection);
        if (it != kConfigSchema.end())
        {
            const auto& allowedKeys = it->second;

            if (allowedKeys.find(key) == allowedKeys.end())
            {
                parseErrors.push_back(ConfigError::UnknownKey(currentSection, key));
                continue;
            }
        }

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
                    parseErrors.push_back(ConfigError::InvalidValue(currentSection, key, value, "integer"));
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
                    parseErrors.push_back(ConfigError::InvalidValue(currentSection, key, value, "double"));
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
                    parseErrors.push_back(ConfigError::InvalidValue(currentSection, key, value, "double"));
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
                    parseErrors.push_back(ConfigError::InvalidValue(currentSection, key, value, "integer"));
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
                    parseErrors.push_back(ConfigError::InvalidValue(currentSection, key, value, "double"));
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
                    parseErrors.push_back(ConfigError::InvalidValue(currentSection, key, value, "double"));
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
                    parseErrors.push_back(ConfigError::InvalidValue(currentSection, key, value, "integer"));
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
                    parseErrors.push_back(ConfigError::InvalidValue(currentSection, key, value, "integer"));
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
                    parseErrors.push_back(ConfigError::InvalidValue(currentSection, key, value, "DEBUG, INFO, WARNING, ERROR"));
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
                    parseErrors.push_back(ConfigError::InvalidValue(currentSection, key, value, "true/false, 1/0"));
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
                    parseErrors.push_back(ConfigError::InvalidValue(currentSection, key, value, "true/false, 1/0"));
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
                    parseErrors.push_back(ConfigError::InvalidValue(currentSection, key, value, "true/false, 1/0"));
                }
            }
        }
    }

    std::vector<std::string> errors;

    if (!parseErrors.empty())
    {
        errors = parseErrors;
    }
   
    auto validationErrors = ConfigValidator::Validate(tempConfig);
    
    if (!validationErrors.empty())
    {
        errors.insert(errors.end(), validationErrors.begin(), validationErrors.end());
    }


    Config& config = Config::GetInstance();
    if (!errors.empty())
    {
        Logger::GetInstance().Log("Config validation failed:", LogLevel::ERR);

        for (const auto& err : errors)
        {
            Logger::GetInstance().Log(err, LogLevel::ERR);
        }

        config.Apply(Config()); // DEFAULT RESET
        Logger::GetInstance().Log("Using default config values.", LogLevel::WARNING);
    }
    else
    {
        config.Apply(tempConfig);
        Logger::GetInstance().Log("Config file loaded successfully.", LogLevel::INFO);
    }


    return true;
}
