/**
 * @file ServerConfig.h
 * @brief Разбор параметров командной строки
 * @details Парсит аргументы командной строки, задаёт значения по умолчанию,
 *          проверяет обязательный флаг --run
 * @author Влазнев К.П.
 * @version 1.0
 * @date 2026-11-18
 */
#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "Exceptions.h"
/**
 * @brief Параметры сервера
 */
struct ServerParams {
    std::string dbPath; ///< Путь к базе пользователей
    std::string logPath; ///< Путь к журналу
    uint16_t port; ///< Номер порта
    bool showHelp;  ///< Флаг вывода справки
    bool isRun; ///< Флаг запуска сервера
    
    ServerParams() {
        dbPath = "base/server.conf";  
        logPath = "login/server.log"; 
        port = 33333;  
        showHelp = false;  
        isRun = false;  
    }
};
/**
 * @brief Класс разбора параметров командной строки
 */
class ServerConfig {
public:
	 /**
     * @brief Разбор аргументов командной строки
     * @param argc количество аргументов
     * @param argv массив аргументов
     * @return структура ServerParams с разобранными параметрами
     */
    static ServerParams parseArgs(int argc, char* argv[]);
    static void showHelp();
};
