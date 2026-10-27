#include "Server.h"
#include "ServerConfig.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        ServerParams params = ServerConfig::parseArgs(argc, argv);
        
        if (params.showHelp) {
            ServerConfig::showHelp();
            return 0;
        }

        Server server(params);
        server.run();

    } catch (const ConfigError& e) {
        std::cerr << "Ошибка конфигурации: " << e.what() << std::endl;
        return 1;
    } catch (const IOError& e) {
        std::cerr << "Ошибка ввода-вывода: " << e.what() << std::endl;
        return 2;
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 3;
    }

    return 0;
}