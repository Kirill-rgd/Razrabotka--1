#include "Server.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sstream>

Server::Server(const ServerParams& p) : params(p), logger(p.logPath), serverSocket(-1) {
    db.load(params.dbPath, logger);
    initSocket();
}

Server::~Server() {
    if (serverSocket != -1) {
        close(serverSocket);
    }
}

void Server::initSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw NetworkError("Ошибка создания сокета");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(params.port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(serverSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        throw NetworkError("Ошибка привязки сокета к порту " + std::to_string(params.port));
    }

    if (listen(serverSocket, 10) < 0) {
        throw NetworkError("Ошибка перевода сокета в режим прослушивания");
    }

    logger.logInfo("Сервер успешно запущен и слушает порт " + std::to_string(params.port));
}

void Server::run() {
    start();
}

void Server::start() {
    logger.logInfo("Ожидание подключений клиентов...");
    while (true) {
        try {
            acceptConnection();
        } catch (const std::exception& e) {
            logger.logError(std::string("Ошибка при обработке клиента: ") + e.what());
        }
    }
}

void Server::acceptConnection() {
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    
    if (clientSocket < 0) {
        logger.logError("Ошибка принятия соединения");
        return;
    }

    std::string clientIP = inet_ntoa(clientAddr.sin_addr);
    logger.logInfo("Установлено соединение с клиентом: " + clientIP);

    try {
        std::string login;
        if (!authenticate(clientSocket, login)) {
            sendText(clientSocket, "ERR");
            logger.logWarning("Ошибка аутентификации клиента. Соединение разорвано.");
        } else {
            sendText(clientSocket, "OK");
            logger.logInfo("Клиент " + login + " успешно авторизован. Начало передачи данных.");
            processVectors(clientSocket);
            logger.logInfo("Все векторы от клиента " + login + " успешно обработаны.");
        }
    } catch (const std::exception& e) {
        logger.logError(std::string("Ошибка при обработке клиента: ") + e.what());
    }

    close(clientSocket);
    logger.logInfo("Соединение с клиентом разорвано.");
}

bool Server::authenticate(int clientSocket, std::string& login) {
    std::string msg = receiveText(clientSocket);
    if (msg.empty()) return false;

    std::string salt, hash;
    Authenticator::parseAuthMessage(msg, login, salt, hash);

    std::string password;
    if (!db.findUser(login, password)) {
        logger.logWarning("Попытка авторизации с неизвестным логином: " + login);
        return false;
    }

    if (Authenticator::verify(salt, password, hash)) {
        return true;
    }
    
    logger.logWarning("Неверный хеш пароля для логина: " + login);
    return false;
}

void Server::processVectors(int clientSocket) {
    uint32_t n = 0;
    receiveBinary(clientSocket, &n, sizeof(n));
    
    logger.logInfo("Получено количество векторов: " + std::to_string(n));

    for (uint32_t i = 0; i < n; ++i) {
        uint32_t s = 0;
        receiveBinary(clientSocket, &s, sizeof(s));
        
        logger.logInfo("Вектор " + std::to_string(i+1) + ", размер: " + std::to_string(s));
        
        std::vector<uint32_t> vec(s);
        if (s > 0) {
            receiveBinary(clientSocket, vec.data(), s * sizeof(uint32_t));
            
            std::string vecData = "Данные вектора " + std::to_string(i+1) + ": ";
            for (uint32_t j = 0; j < s; ++j) {
                vecData += std::to_string(vec[j]);
                if (j < s - 1) vecData += ", ";
            }
            logger.logInfo(vecData);
        }

        uint32_t sum = VectorProcessor::sumVector(vec, logger);
        logger.logInfo("Сумма вектора " + std::to_string(i+1) + ": " + std::to_string(sum));
        
        sendBinary(clientSocket, &sum, sizeof(sum));
    }

    logger.logInfo("Все векторы обработаны.");
}

std::string Server::receiveText(int clientSocket) {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) return "";
    buffer[bytes] = '\0';
    return std::string(buffer);
}

void Server::sendText(int clientSocket, const std::string& msg) {
    send(clientSocket, msg.c_str(), msg.length(), MSG_NOSIGNAL);
}

void Server::sendBinary(int clientSocket, const void* data, size_t size) {
    size_t total = 0;
    const char* ptr = static_cast<const char*>(data);
    while (total < size) {
        ssize_t bytes = send(clientSocket, ptr + total, size - total, MSG_NOSIGNAL);
        if (bytes <= 0) throw NetworkError("Разрыв соединения при отправке бинарных данных");
        total += bytes;
    }
}

void Server::receiveBinary(int clientSocket, void* data, size_t size) {
    size_t total = 0;
    char* ptr = static_cast<char*>(data);
    while (total < size) {
        ssize_t bytes = recv(clientSocket, ptr + total, size - total, 0);
        if (bytes <= 0) throw NetworkError("Разрыв соединения при приеме бинарных данных");
        total += bytes;
    }
}