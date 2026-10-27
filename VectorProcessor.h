#pragma once
#include <vector>
#include <cstdint>
#include "Logger.h"

class VectorProcessor {
public:
    static uint32_t sumVector(const std::vector<uint32_t>& vec, Logger& logger);
};