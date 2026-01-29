#include <string>
#include <mutex>
#include "LogLevel.h"

class Logger
{
private:
    std::string m_filename;
    std::mutex m_mutex;

    Logger(); // private constructor

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& GetInstance();

    void Log(const std::string& message, LogLevel level = LogLevel::INFO);
};