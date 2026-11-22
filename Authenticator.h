/**
 * @file Authenticator.h
 * @brief Модуль аутентификации с использованием SHA-224
 * @details Реализует схему аутентификации с солью: HASH = SHA224(SALT || PASSWORD).
 *          Соль и хеш представляются в виде hex-строк фиксированной длины.
 * @author Влазнев К.П.
 * @version 1.0
 * @date 2026-11-19
 */
#pragma once
#include <string>
#include "Logger.h"
/**
 * @brief Класс аутентификации
 * @details Предоставляет статические методы для генерации соли,
 *          вычисления и проверки хешей, а также разбора аутентификационных сообщений.
 */
class Authenticator {
public:
	/**
     * @brief Генерация случайной соли длиной 16 символов (hex)
     * @return Строка с шестнадцатеричным представлением соли
     */
    static std::string generateSalt16();
     /**
     * @brief Вычисление хеша SHA-224 от конкатенации соли и пароля
     * @param salt16 соль в виде hex-строки (16 символов)
     * @param password пароль клиента
     * @return хеш в виде hex-строки (56 символов)
     */
    static std::string computeHash(const std::string& salt16, const std::string& password);
     /**
     * @brief Проверка хеша с использованием HashVerificationFilter
     * @param salt16 соль
     * @param password пароль
     * @param receivedHash ожидаемый хеш от клиента
     * @return true если хеш совпадает, false иначе
     */
    static bool verify(const std::string& salt16, const std::string& password, const std::string& receivedHash);
    /**
     * @brief Разбор аутентификационного сообщения
     * @param msg сообщение формата LOGIN+SALT+HASH
     * @param login[out] извлечённый логин
     * @param salt[out] извлечённая соль (16 символов)
     * @param hash[out] извлечённый хеш (56 символов)
     */
    static void parseAuthMessage(const std::string& msg, std::string& login, std::string& salt, std::string& hash);
};
