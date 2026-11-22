/**
 * @file main.cpp
 * @brief Точка входа сервера обработки векторов
 * @details Сетевой сервер с подсистемой аутентификации SHA-224.
 *          Принимает подключения клиентов, выполняет аутентификацию
 *          и вычисляет сумму векторов uint32_t.
 *          Все ошибки логируются в файл журнала, а критические
 *          выводятся в стандартный поток ошибок.
 * @warning Требуется обязательный параметр --run для запуска сервера
 * @author Влазнев К.П.
 * @version 1.0
 * @date 2026-11-20
 */
#include "Server.h"
#include "ServerConfig.h"
#include "Exceptions.h"
#include <iostream>
/**
 * @brief Точка входа программы
 * @details Парсит параметры командной строки, выводит справку при необходимости,
 *          создаёт и запускает сервер. Обрабатывает исключения сетевого уровня,
 *          конфигурации и общие исключения std::exception.
 * @param argc количество аргументов командной строки
 * @param argv массив строк с аргументами
 * @return 0 при успешном завершении, 1 при ошибке
 */
int main(int argc, char* argv[]) {
    try {
        ServerParams params = ServerConfig::parseArgs(argc, argv);
        
        if (params.showHelp) {
            ServerConfig::showHelp();
            return 0;
        }

        if (!params.isRun) {
            std::cerr << "Ошибка: Для запуска сервера необходимо указать параметр --run" << std::endl;
            ServerConfig::showHelp();
            return 1;
        }

        Server server(params);
        server.run();
        
    } catch (const NetworkError& e) {
        std::cerr << "Сетевая ошибка: " << e.what() << std::endl;
        return 1;
    } catch (const ConfigError& e) {
        std::cerr << "Ошибка конфигурации: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
