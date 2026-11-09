#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <cstdio>
#include <chrono>

#include "ServerConfig.h"
#include "AuthDatabase.h"
#include "Authenticator.h"
#include "VectorProcessor.h"
#include "Logger.h"

using namespace std;

int total = 0, passed = 0, failed = 0;

void check(int n, const string& desc, bool cond) {
    total++;
    cout << "T" << n << " RUN" << endl;
    if (cond) {
        cout << "T" << n << " PASS" << endl;
        passed++;
    } else {
        cout << "T" << n << " FAIL: " << desc << endl;
        failed++;
        assert(cond);
    }
}

bool log_contains(const string& file, const string& text, const string& tag) {
    ifstream f(file);
    string line;
    while (getline(f, line))
        if (line.find(text) != string::npos && line.find(tag) != string::npos)
            return true;
    return false;
}

void t1() {
    Logger l("t.log");
    vector<uint32_t> v;
    check(1, "empty vector", VectorProcessor::sumVector(v, l) == 0);
    remove("t.log");
}

void t2() {
    Logger l("t.log");
    vector<uint32_t> v = {999999};
    check(2, "one element", VectorProcessor::sumVector(v, l) == 999999);
    remove("t.log");
}

void t3() {
    Logger l("t.log");
    vector<uint32_t> v = {100, 200, 300, 400};
    check(3, "basic sum", VectorProcessor::sumVector(v, l) == 1000);
    remove("t.log");
}

void t4() {
    Logger l("t.log");
    vector<uint32_t> v = {1000000, 2000000, 3000000};
    check(4, "large values", VectorProcessor::sumVector(v, l) == 6000000);
    remove("t.log");
}

void t5() {
    Logger l("t.log");
    vector<uint32_t> v = {4294967290, 10};
    check(5, "overflow up", VectorProcessor::sumVector(v, l) == 4294967295);
    remove("t.log");
}

void t6() {
    Logger l("t.log");
    vector<uint32_t> v = {0, 0, 0};
    check(6, "no overflow down", VectorProcessor::sumVector(v, l) == 0);
    remove("t.log");
}

void t7() {
    Logger l("t.log");
    vector<uint32_t> v = {4294967295};
    check(7, "max uint32", VectorProcessor::sumVector(v, l) == 4294967295);
    remove("t.log");
}

void t8() {
    Logger l("t.log");
    vector<uint32_t> v = {2147483647, 2147483648};
    check(8, "exact max", VectorProcessor::sumVector(v, l) == 4294967295);
    remove("t.log");
}

void t9() {
    string h = Authenticator::computeHash("A1B2C3D4E5F60789", "TestPass123");
    check(9, "hash length", h.length() == 56);
}

void t10() {
    string s = "A1B2C3D4E5F60789", p = "TestPass123";
    string h = Authenticator::computeHash(s, p);
    check(10, "verify ok", Authenticator::verify(s, p, h) == true);
}

void t11() {
    string s = "A1B2C3D4E5F60789", p = "TestPass123";
    string h = Authenticator::computeHash(s, p);
    check(11, "verify wrong", Authenticator::verify(s, "Wrong", h) == false);
}

void t12() {
    string login, salt, hash;
    Authenticator::parseAuthMessage(
        string("admin") + "7F8E9D0C1B2A3456" + string(56, 'A'),
        login, salt, hash);
    check(12, "parse login", login == "admin");
    check(13, "parse salt len", salt.length() == 16);
    check(14, "parse hash len", hash.length() == 56);
}

void t15() {
    string h1 = Authenticator::computeHash("1111111111111111", "Same");
    string h2 = Authenticator::computeHash("2222222222222222", "Same");
    check(15, "diff salts", h1 != h2);
}

void t16() {
    string h = Authenticator::computeHash("A1B2C3D4E5F60789", "");
    check(16, "empty pass", h.length() == 56);
}

void t17() {
    ofstream f("db.conf"); f << "user1:pass1\n"; f.close();
    Logger l("t.log"); AuthDatabase db; db.load("db.conf", l);
    string p;
    check(17, "one user", db.findUser("user1", p) && p == "pass1");
    remove("db.conf"); remove("t.log");
}

void t18() {
    ofstream f("db.conf"); f << "u1:p1\nu2:p2\nu3:p3\n"; f.close();
    Logger l("t.log"); AuthDatabase db; db.load("db.conf", l);
    string p;
    check(18, "multi users", db.findUser("u2", p) && !db.findUser("u4", p));
    remove("db.conf"); remove("t.log");
}

void t19() {
    ofstream f("db.conf"); f.close();
    Logger l("t.log"); AuthDatabase db; db.load("db.conf", l);
    string p;
    check(19, "empty file", !db.findUser("x", p));
    remove("db.conf"); remove("t.log");
}

void t20() {
    Logger l("t.log"); AuthDatabase db;
    bool caught = false;
    try { db.load("no.conf", l); } catch (const IOError&) { caught = true; }
    check(20, "no file", caught);
    remove("t.log");
}

void t21() {
    ofstream f("db.conf"); f << "user:P@ssW0rd!#$\n"; f.close();
    Logger l("t.log"); AuthDatabase db; db.load("db.conf", l);
    string p;
    check(21, "special chars", db.findUser("user", p) && p == "P@ssW0rd!#$");
    remove("db.conf"); remove("t.log");
}

void t22() {
    char* a[] = {(char*)"s", nullptr};
    ServerParams p = ServerConfig::parseArgs(1, a);
    check(22, "no args", p.showHelp);
}

void t23() {
    char* a[] = {(char*)"s", (char*)"-h", nullptr};
    ServerParams p = ServerConfig::parseArgs(2, a);
    check(23, "-h flag", p.showHelp);
}

void t24() {
    char* a[] = {(char*)"s", (char*)"--help", nullptr};
    ServerParams p = ServerConfig::parseArgs(2, a);
    check(24, "--help flag", p.showHelp);
}

void t25() {
    char* a[] = {(char*)"s", (char*)"--run", nullptr};
    ServerParams p = ServerConfig::parseArgs(2, a);
    check(25, "--run flag", p.isRun && !p.showHelp);
}

void t26() {
    char* a[] = {(char*)"s", (char*)"--run", (char*)"-p", (char*)"9090", nullptr};
    ServerParams p = ServerConfig::parseArgs(4, a);
    check(26, "custom port", p.port == 9090);
}

void t27() {
    char* a[] = {(char*)"s", (char*)"--run",
                 (char*)"-db", (char*)"x.conf",
                 (char*)"-log", (char*)"x.log", nullptr};
    ServerParams p = ServerConfig::parseArgs(6, a);
    check(27, "custom paths", p.dbPath == "x.conf" && p.logPath == "x.log");
}

void t28() {
    char* a[] = {(char*)"s", (char*)"-p", (char*)"33333", nullptr};
    ServerParams p = ServerConfig::parseArgs(3, a);
    check(28, "no --run", p.showHelp && !p.isRun);
}

void t29() {
    ServerParams p;
    check(29, "defaults", p.port == 33333 && p.dbPath == "base/server.conf");
}

void t30() {
    char* a[] = {(char*)"s", (char*)"--run", (char*)"-h", nullptr};
    ServerParams p = ServerConfig::parseArgs(3, a);
    check(30, "--run then -h", p.showHelp);
}

void t31() {
    Logger l("t.log"); l.logInfo("msg1");
    check(31, "INFO", log_contains("t.log", "msg1", "[INFO]"));
    remove("t.log");
}

void t32() {
    Logger l("t.log"); l.logWarning("msg2");
    check(32, "WARNING", log_contains("t.log", "msg2", "[WARNING]"));
    remove("t.log");
}

void t33() {
    Logger l("t.log"); l.logError("msg3");
    check(33, "ERROR", log_contains("t.log", "msg3", "[ERROR]"));
    remove("t.log");
}

void t34() {
    Logger l("t.log"); l.logInfo("date test");
    ifstream f("t.log"); string line; getline(f, line);
    check(34, "timestamp", line.find("[20") != string::npos);
    remove("t.log");
}

void t35() {
    Logger l("t.log");
    l.logInfo("first"); l.logWarning("second"); l.logError("third");
    check(35, "multi logs",
          log_contains("t.log", "first", "[INFO]") &&
          log_contains("t.log", "third", "[ERROR]"));
    remove("t.log");
}

void t36() {
    Logger l("t.log");
    l.logInfo(string(500, 'A'));
    check(36, "long msg", log_contains("t.log", string(500, 'A'), "[INFO]"));
    remove("t.log");
}

void t37() {
    Logger l("t.log");
    l.logInfo("special: <>&\"'");
    check(37, "special chars", log_contains("t.log", "special:", "[INFO]"));
    remove("t.log");
}

int main() {
    auto start = chrono::high_resolution_clock::now();

    cout << "=== МОДУЛЬНОЕ ТЕСТИРОВАНИЕ TIMP_Server ===" << endl << endl;

    cout << "[VectorProcessor]" << endl;
    t1(); t2(); t3(); t4(); t5(); t6(); t7(); t8();
    cout << endl;

    cout << "[Authenticator]" << endl;
    t9(); t10(); t11(); t12(); t15(); t16();
    cout << endl;

    cout << "[AuthDatabase]" << endl;
    t17(); t18(); t19(); t20(); t21();
    cout << endl;

    cout << "[ServerConfig]" << endl;
    t22(); t23(); t24(); t25(); t26(); t27(); t28(); t29(); t30();
    cout << endl;

    cout << "[Logger]" << endl;
    t31(); t32(); t33(); t34(); t35(); t36(); t37();
    cout << endl;

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "========================================" << endl;
    cout << "РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ" << endl;
    cout << "========================================" << endl;
    cout << "Количество выполненных тестов: " << total << endl;
    cout << "Успешно пройдено:              " << passed << endl;
    cout << "Завершено с ошибкой:           " << failed << endl;
    cout << "Затраченное время:             " << duration.count() / 1000.0 << " сек" << endl;
    cout << "========================================" << endl;

    return failed > 0 ? 1 : 0;
}