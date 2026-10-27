#pragma once
#include <string>
#include <map>
#include "Exceptions.h"
#include "Logger.h"

class AuthDatabase {
private:
    std::map<std::string, std::string> users;
public:
    void load(const std::string& path, Logger& logger);
    bool findUser(const std::string& login, std::string& password) const;
};