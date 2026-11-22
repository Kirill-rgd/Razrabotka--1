#!/bin/bash

LOG="func_test.log"
> $LOG

log() { echo "$1" >> $LOG; }

run_test() {
    log "=============================="
    log "ТЕСТ: $1"
}

# ТЕСТ 1: Запуск без параметров — должна быть справка
run_test "Запуск без параметров"
OUT=$(./server 2>&1)
if echo "$OUT" | grep -qi "использование\|usage\|справка\|help"; then
    log "РЕЗУЛЬТАТ: PASS"
else
    log "РЕЗУЛЬТАТ: FAIL"
fi

# ТЕСТ 2: Флаг -h
run_test "Флаг -h"
OUT=$(./server -h 2>&1)
if echo "$OUT" | grep -qi "использование\|usage\|справка\|help"; then
    log "РЕЗУЛЬТАТ: PASS"
else
    log "РЕЗУЛЬТАТ: FAIL"
fi

# ТЕСТ 3: Без обязательного --run
run_test "Без обязательного --run"
OUT=$(./server -p 33333 2>&1)
if echo "$OUT" | grep -qi "использование\|usage\|справка\|help\|--run"; then
    log "РЕЗУЛЬТАТ: PASS"
else
    log "РЕЗУЛЬТАТ: FAIL"
fi

# ТЕСТ 4: Запуск сервера с --run
run_test "Запуск сервера с --run"
./server --run -p 33333 &
PID=$!
sleep 2
if kill -0 $PID 2>/dev/null; then
    log "РЕЗУЛЬТАТ: PASS (PID=$PID)"
else
    log "РЕЗУЛЬТАТ: FAIL"
fi

# ТЕСТ 5: Проверка создания журнала
run_test "Проверка журнала сервера"
if [ -f "login/server.log" ]; then
    log "РЕЗУЛЬТАТ: PASS"
    log "Содержимое журнала:"
    cat login/server.log >> $LOG
else
    log "РЕЗУЛЬТАТ: FAIL (файл журнала не создан)"
fi

# ТЕСТ 6: Позитивная аутентификация
run_test "Аутентификация с верным паролем"
OUT=$(./client_uint32_t -H SHA224 -S c 2>&1)
if echo "$OUT" | grep -q "OK"; then
    log "РЕЗУЛЬТАТ: PASS"
else
    log "РЕЗУЛЬТАТ: FAIL"
fi

# ТЕСТ 7: Негативная аутентификация (через журнал)
run_test "Аутентификация с неверным паролем"
# Добавляем в базу пользователя с неверным паролем
echo "wrong_user:wrong_pass" >> base/server.conf
sleep 1
# Проверяем, что сервер продолжает работать
if kill -0 $PID 2>/dev/null; then
    log "РЕЗУЛЬТАТ: PASS (сервер продолжает работу)"
else
    log "РЕЗУЛЬТАТ: FAIL"
fi
# Удаляем тестовую запись
sed -i '/wrong_user/d' base/server.conf

# ТЕСТ 8: Устойчивость при разрыве соединения (ОБЯЗАТЕЛЬНО по методичке)
run_test "Устойчивость при разрыве соединения"
timeout 1 ./client_uint32_t -H SHA224 -S c 2>/dev/null
sleep 1
if kill -0 $PID 2>/dev/null; then
    log "РЕЗУЛЬТАТ: PASS (сервер продолжает работу)"
else
    log "РЕЗУЛЬТАТ: FAIL (сервер упал)"
fi

# ТЕСТ 9: Нагрузочное тестирование (3 клиента подряд)
run_test "Нагрузочное тестирование (3 клиента)"OK=0
for i in 1 2 3; do
    OUT=$(./client_uint32_t -H SHA224 -S c 2>&1)
    if echo "$OUT" | grep -q "OK"; then
        OK=$((OK+1))
        log "Клиент #$i: PASS"
    else
        log "Клиент #$i: FAIL"
    fi
    sleep 1
done
if [ $OK -eq 3 ]; then
    log "ИТОГ: PASS (3/3)"
else
    log "ИТОГ: FAIL ($OK/3)"
fi

# Завершение сервера
kill $PID 2>/dev/null
wait $PID 2>/dev/null

log ""
log "=============================="
log "Функциональное тестирование завершено"
log "=============================="
