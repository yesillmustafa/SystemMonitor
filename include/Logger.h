#include <string>
#include <mutex>
#include <vector>
#include <memory>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>
#include "LogLevel.h"
#include "ILogOutput.h"

class Logger
{
public:

    static Logger& GetInstance();

    void Log(const std::string& message, LogLevel level = LogLevel::INFO);

    void Shutdown();   // Graceful shutdown için

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void AddOutput(std::unique_ptr<ILogOutput> output);

    std::string FormatMessage(const std::string& message, LogLevel level);
    std::string FormatTimestamp();
    std::string LevelToString(LogLevel level);

    void WorkerLoop();

private:
    std::vector<std::unique_ptr<ILogOutput>> m_outputs;
    LogLevel m_minLevel;

    // Async logging
    std::queue<std::string> m_queue;
    std::mutex m_queueMutex;
    std::condition_variable m_cv;
    std::thread m_worker;
    std::atomic<bool> m_running{ false };
};