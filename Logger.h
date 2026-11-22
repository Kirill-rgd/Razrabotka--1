/**
 * @file Logger.h
 * @brief Модуль журналирования событий сервера
 * @details Записывает сообщения разных уровней критичности в файл журнала
 *          с временными метками. Для потокобезопасности используется mutex.
 * @author Влазнев К.П.
 * @version 1.0
 * @date 2026-11-18
 */
#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <ctime>
#include <iomanip>
#include <sstream>
/**
 * @brief Уровни критичности сообщений журнала
 */
enum class LogLevel {
    INFO, ///< Информационное сообщение
    WARNING,  ///< Предупреждение
    ERROR  ///< Ошибка
};
/**
 * @brief Класс для записи событий в файл журнала
 */
class Logger {
private:
    std::string logFilePath;  ///< Путь к файлу журнала
    std::ofstream logFile; ///< Поток для записи в файл
    std::mutex logMutex;  ///< Мьютекс для потокобезопасности

    std::string getCurrentTime();   ///< Получить текущую дату и время
    std::string levelToString(LogLevel level);  ///< Преобразовать уровень в строку

public:
	 /**
     * @brief Конструктор, открывает файл журнала
     * @param path путь к файлу журнала
     * @throw IOError если файл не удалось открыть
     */
    explicit Logger(const std::string& path);
     /**
     * @brief Деструктор, закрывает файл журнала
     */
    ~Logger();
	/**
     * @brief Запись сообщения в журнал
     * @param level уровень критичности
     * @param message текст сообщения
     */
    void log(LogLevel level, const std::string& message);
    /**
     * @brief Запись информационного сообщения
     * @param message текст сообщения
     */
    void logInfo(const std::string& message);
    /**
     * @brief Запись предупреждения
     * @param message текст сообщения
     */
    void logWarning(const std::string& message);
     /**
     * @brief Запись ошибки
     * @param message текст сообщения
     */
    void logError(const std::string& message);
};
