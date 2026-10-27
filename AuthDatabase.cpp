#include "AuthDatabase.h"
#include <fstream>
#include <sstream>

void AuthDatabase::load(const std::string& path, Logger& logger) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw IOError("Не удалось открыть базу данных: " + path);
    }

    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            users[line.substr(0, pos)] = line.substr(pos + 1);
            count++;
        }
    }
    logger.logInfo("База клиентов загружена. Найдено пользователей: " + std::to_string(count));
}

bool AuthDatabase::findUser(const std::string& login, std::string& password) const {
    auto it = users.find(login);
    if (it != users.end()) {
        password = it->second;
        return true;
    }
    return false;
}