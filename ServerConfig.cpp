#include "ServerConfig.h"
#include <iostream>
#include <cstring>

void ServerConfig::showHelp() {
    std::cout << "Использование: ./server [options]\n"
              << "  -d <path>   Путь к базе клиентов (по умолч.: base/server.conf)\n"
              << "  -l <path>   Путь к файлу журнала (по умолч.: login/server.log)\n"
              << "  -p <port>   Номер порта (по умолч.: 33333)\n"
              << "  -h          Показать справку\n";
}

ServerParams ServerConfig::parseArgs(int argc, char* argv[]) {
    ServerParams params;

    if (argc == 1) {
        params.showHelp = true;
        return params;
    }

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0) {
            params.showHelp = true;
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            params.dbPath = argv[++i];
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            params.logPath = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            params.port = std::stoi(argv[++i]);
        } else {
            throw ConfigError(std::string("Неизвестный параметр: ") + argv[i]);
        }
    }
    return params;
}