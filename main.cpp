#include "Server.h"
#include "ServerConfig.h"
#include "Exceptions.h"
#include <iostream>

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