/**
 * @file VectorProcessor.h
 * @brief Модуль обработки векторов
 * @details Вычисляет сумму элементов вектора типа uint32_t
 *          с защитой от целочисленного переполнения.
 * @author Влазнев К.П.
 * @version 1.0
 * @date 2026-11-20
 */
#pragma once
#include <vector>
#include <cstdint>
#include "Logger.h"
/**
 * @brief Класс обработки векторов
 */
class VectorProcessor {
public:
	 /**
     * @brief Вычисление суммы элементов вектора
     * @param vec вектор элементов типа uint32_t
     * @param logger объект журнала для записи предупреждений о переполнении
     * @return сумма элементов. При переполнении возвращается UINT32_MAX
     */
    static uint32_t sumVector(const std::vector<uint32_t>& vec, Logger& logger);
};
