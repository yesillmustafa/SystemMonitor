#include <string>
#include <mutex>
#include "LogLevel.h"

class Logger
{
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& GetInstance();

    void Log(const std::string& message, LogLevel level = LogLevel::INFO);

private:
    std::string m_filename;
    std::mutex m_mutex;
    LogLevel m_minLevel;

    Logger(); // private constructor

    std::string FormatTimestamp();
    std::string LevelToString(LogLevel level);
};