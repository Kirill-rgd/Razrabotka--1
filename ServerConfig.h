#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "Exceptions.h"

struct ServerParams {
    std::string dbPath;
    std::string logPath;
    uint16_t port;
    bool showHelp;
    bool isRun;
    
    ServerParams() {
        dbPath = "base/server.conf";
        logPath = "login/server.log";
        port = 33333;
        showHelp = false;
        isRun = false;
    }
};

class ServerConfig {
public:
    static ServerParams parseArgs(int argc, char* argv[]);
    static void showHelp();
};