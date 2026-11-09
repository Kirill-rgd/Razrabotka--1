#include "ServerConfig.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

ServerParams ServerConfig::parseArgs(int argc, char* argv[]) {
    ServerParams params;
    
    bool hasRun = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--run") == 0) {
            hasRun = true;
            break;
        }
    }

    if (argc == 1 || !hasRun) {
        params.showHelp = true;
        return params;
    }
    
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            params.showHelp = true;
            return params;
        } else if (strcmp(argv[i], "--run") == 0) {
            params.isRun = true;
        } else if (strcmp(argv[i], "-db") == 0 && i + 1 < argc) {
            params.dbPath = argv[++i];
        } else if (strcmp(argv[i], "-log") == 0 && i + 1 < argc) {
            params.logPath = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            params.port = static_cast<uint16_t>(std::stoi(argv[++i]));
        }
    }
    
    return params;
}

void ServerConfig::showHelp() {
    std::cout << "Использование: server --run [options]\n"
              << "Обязательный параметр: --run (разрешение на запуск)\n"
              << "Options:\n"
              << "  -h, --help      Показать эту справку\n"
              << "  -db <file>      Путь к файлу базы пользователей (по умолчанию: base/server.conf)\n"
              << "  -log <file>     Путь к файлу журнала (по умолчанию: login/server.log)\n"
              << "  -p <port>       Номер порта (по умолчанию: 33333)\n";
}