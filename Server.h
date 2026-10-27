#pragma once
#include "ServerConfig.h"
#include "Logger.h"
#include "AuthDatabase.h"
#include "Authenticator.h"
#include "VectorProcessor.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cstdint>

class Server {
private:
    ServerParams params;
    Logger logger;
    AuthDatabase db;
    int serverSocket;

    void initSocket();
    void start();
    void acceptConnection();
    bool authenticate(int clientSocket, std::string& login);
    void processVectors(int clientSocket);
    std::string receiveText(int clientSocket);
    void sendText(int clientSocket, const std::string& msg);
    void sendBinary(int clientSocket, const void* data, size_t size);
    void receiveBinary(int clientSocket, void* data, size_t size);

public:
    Server(const ServerParams& p);
    ~Server();
    void run();
};