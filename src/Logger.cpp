#include "Logger.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include "Config.h"
#include "FileOutput.h"
#include "ConsoleOutput.h"

Logger::Logger()
{
    const auto& config = Config::GetInstance().Logger();
    m_minLevel = config.minLevel;

    if (config.enableFileLog)
        AddOutput(std::make_unique<FileOutput>(config.filePath));

    if (config.enableConsoleLog)
        AddOutput(std::make_unique<ConsoleOutput>());

    m_running = true;
    m_worker = std::thread(&Logger::WorkerLoop, this);
}

Logger::~Logger()
{
    Shutdown();
}

Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

void Logger::AddOutput(std::unique_ptr<ILogOutput> output)
{
    m_outputs.push_back(std::move(output));
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

std::string Logger::FormatMessage(const std::string& message, LogLevel level)
{
    std::ostringstream oss;
    oss << "[" << FormatTimestamp() << "] "
        << "[" << LevelToString(level) << "] "
        << message;
    return oss.str();
}

void Logger::Log(const std::string& message, LogLevel level)
{
    if (level < m_minLevel)
        return;

    std::string formatted = FormatMessage(message, level);

    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_queue.push(std::move(formatted));
    }
    
    m_cv.notify_one(); // Logger threadi uyandýr
}

void Logger::WorkerLoop()
{
    while (m_running || !m_queue.empty())
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);

        m_cv.wait(lock, [&]() {
            return !m_running || !m_queue.empty();
        });

        while (!m_queue.empty())
        {
            std::string msg = std::move(m_queue.front());
            m_queue.pop();
            lock.unlock();

            for (auto& output : m_outputs)
            {
                output->Write(msg);
            }
            
            lock.lock();
        }
    }
}

void Logger::Shutdown()
{
    bool expected = true;
    if (!m_running.compare_exchange_strong(expected, false))
        return; // zaten kapanmýþ

    m_cv.notify_all();
    
    if (m_worker.joinable())
        m_worker.join();
}