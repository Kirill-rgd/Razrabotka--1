#include "Logger.h"
#include "Exceptions.h"
#include <iostream>

Logger::Logger(const std::string& path) : logFilePath(path) {
    logFile.open(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        throw IOError("Не удалось открыть файл журнала: " + logFilePath);
    }
    logInfo("Файл журнала инициализирован.");
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

std::string Logger::getCurrentTime() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTime() << "] [" 
                << levelToString(level) << "] " << message << "\n";
        logFile.flush();
    }
}

void Logger::logInfo(const std::string& message) { log(LogLevel::INFO, message); }
void Logger::logWarning(const std::string& message) { log(LogLevel::WARNING, message); }
void Logger::logError(const std::string& message) { log(LogLevel::ERROR, message); }