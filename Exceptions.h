#pragma once
#include <stdexcept>
#include <string>

class ProgramError : public std::runtime_error {
public:
    explicit ProgramError(const std::string& msg) : std::runtime_error(msg) {}
};

class ConfigError : public ProgramError {
public:
    explicit ConfigError(const std::string& msg) : ProgramError(msg) {}
};

class IOError : public ProgramError {
public:
    explicit IOError(const std::string& msg) : ProgramError(msg) {}
};

class NetworkError : public ProgramError {
public:
    explicit NetworkError(const std::string& msg) : ProgramError(msg) {}
};

class AuthError : public ProgramError {
public:
    explicit AuthError(const std::string& msg) : ProgramError(msg) {}
};