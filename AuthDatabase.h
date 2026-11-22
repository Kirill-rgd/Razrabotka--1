/**
 * @file AuthDatabase.h
 * @brief Модуль базы данных пользователей
 * @details Загружает пары логин:пароль из текстового файла формата "login:pass".
 *          Обеспечивает поиск пароля по логину для аутентификации.
 * @author Влазнев К.П.
 * @version 1.0
 * @date 2026-11-19
 */
#pragma once
#include <string>
#include <map>
#include "Exceptions.h"
#include "Logger.h"
/**
 * @brief Класс базы аутентификации
 * @details Хранит учетные записи в ассоциативном массиве и предоставляет
 *          методы для загрузки из файла и поиска пользователя.
 */
class AuthDatabase {
private:
    std::map<std::string, std::string> users; ///< Карта пар логин-пароль
public:
	/**
     * @brief Загрузка базы пользователей из файла
     * @param path путь к файлу базы (формат: login:password)
     * @param logger объект журнала для записи событий загрузки
     * @throw IOError если файл не найден или не удалось открыть
     */
    void load(const std::string& path, Logger& logger);
    /**
     * @brief Поиск пользователя по логину
     * @param login искомый логин
     * @param password[out] пароль пользователя, если найден
     * @return true если пользователь найден, false иначе
     */
    bool findUser(const std::string& login, std::string& password) const;
};
