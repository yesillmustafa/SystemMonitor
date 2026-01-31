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
    m_filename = config.Logger().filePath;
    m_minLevel = config.Logger().minLevel;

    std::filesystem::path logPath(m_filename);
    if (!std::filesystem::exists(logPath.parent_path()))
        std::filesystem::create_directories(logPath.parent_path());
}

Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

std::string Logger::FormatTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &now_time);
    std::ostringstream timestamp;
    timestamp << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return timestamp.str();
}

std::string Logger::LevelToString(LogLevel level) {
    switch (level) {
    case LogLevel::DEBUG: return "DEBUG";
    case LogLevel::INFO: return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::ERR: return "ERROR";
    default: return "UNKNOWN";
    }
}

void Logger::Log(const std::string& message, LogLevel level)
{
    if (level < m_minLevel) return;

    std::lock_guard<std::mutex> lock(m_mutex);

    std::string formatted = "[" + FormatTimestamp() + "] [" + LevelToString(level) + "] " + message;

    std::ofstream file(m_filename, std::ios::app);

    if (file.is_open())
        file << formatted << std::endl;
    else
        std::cerr << "Failed to open log file: " << m_filename << std::endl;

    std::cout << formatted << std::endl;
}
