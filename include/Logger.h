#include <string>
#include <mutex>
#include <vector>
#include "LogLevel.h"
#include "ILogOutput.h"

class Logger
{
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& GetInstance();

    void Log(const std::string& message, LogLevel level = LogLevel::INFO);
    
    void AddOutput(std::unique_ptr<ILogOutput> output);

private:
    Logger(); // private constructor

    std::vector<std::unique_ptr<ILogOutput>> m_outputs;
    std::mutex m_mutex;
    LogLevel m_minLevel;

    std::string FormatMessage(const std::string& message, LogLevel level);
    std::string FormatTimestamp();
    std::string LevelToString(LogLevel level);
};