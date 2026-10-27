#include "VectorProcessor.h"
#include <climits>

uint32_t VectorProcessor::sumVector(const std::vector<uint32_t>& vec, Logger& logger) {
    uint32_t sum = 0;
    bool overflow = false;

    for (uint32_t val : vec) {
        if (UINT32_MAX - sum < val) {
            sum = UINT32_MAX;
            overflow = true;
            break; 
        }
        sum += val;
    }

    if (overflow) {
        logger.logWarning("Произошло переполнение при суммировании вектора. Результат ограничен значением 2^32-1 (UINT32_MAX).");
    }

    return sum;
}