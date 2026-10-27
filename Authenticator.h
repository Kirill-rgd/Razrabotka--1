#pragma once
#include <string>
#include "Logger.h"

class Authenticator {
public:
    static std::string generateSalt16();
    static std::string computeHash(const std::string& salt16, const std::string& password);
    static bool verify(const std::string& salt16, const std::string& password, const std::string& receivedHash);
    static void parseAuthMessage(const std::string& msg, std::string& login, std::string& salt, std::string& hash);
};