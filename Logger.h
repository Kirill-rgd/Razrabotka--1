#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <ctime>
#include <iomanip>
#include <sstream>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    std::string logFilePath;
    std::ofstream logFile;
    std::mutex logMutex;

    std::string getCurrentTime();
    std::string levelToString(LogLevel level);

public:
    explicit Logger(const std::string& path);
    ~Logger();

    void log(LogLevel level, const std::string& message);
    void logInfo(const std::string& message);
    void logWarning(const std::string& message);
    void logError(const std::string& message);
};