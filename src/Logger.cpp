#include "Logger.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include "Config.h"

Logger::Logger()
{
    const auto& config = Config::GetInstance();
    m_filename = config.GetLogFilePath();
    m_minLevel = config.GetMinLogLevel();

    std::filesystem::path logPath(m_filename);
    if (!std::filesystem::exists(logPath.parent_path()))
        std::filesystem::create_directories(logPath.parent_path());
}

Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

void Logger::Log(const std::string& message, LogLevel level)
{
    if (level < m_minLevel)
        return;

    std::lock_guard<std::mutex> lock(m_mutex);

    std::ofstream file(m_filename, std::ios::app);
    if (!file.is_open())
    {
        std::cerr << "Failed to open log file: " << m_filename << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &now_time);

    std::ostringstream timestamp;
    timestamp << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    std::string levelStr;
    switch (level)
    {
    case LogLevel::INFO: levelStr = "INFO"; break;
    case LogLevel::WARNING: levelStr = "WARNING"; break;
    case LogLevel::ERR: levelStr = "ERROR"; break;
    default: levelStr = "UNKNOWN"; break;
    }

    file << "[" << timestamp.str() << "] [" << levelStr << "] " << message << std::endl;
}
