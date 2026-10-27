#include "Authenticator.h"
#include "Exceptions.h"
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <iomanip>
#include <sstream>

std::string Authenticator::generateSalt16() {
    CryptoPP::AutoSeededRandomPool prng;
    uint64_t salt64;
    prng.GenerateBlock(reinterpret_cast<CryptoPP::byte*>(&salt64), sizeof(salt64));
    
    std::ostringstream oss;
    oss << std::hex << std::setw(16) << std::setfill('0') << salt64;
    std::string result = oss.str();
    for(auto & c: result) c = toupper(c);
    return result;
}

std::string Authenticator::computeHash(const std::string& salt16, const std::string& password) {
    std::string data = salt16 + password;
    CryptoPP::SHA224 sha224;
    CryptoPP::byte digest[CryptoPP::SHA224::DIGESTSIZE];
    
    sha224.CalculateDigest(digest, reinterpret_cast<const CryptoPP::byte*>(data.data()), data.size());
    
    std::string hexDigest;
    CryptoPP::HexEncoder encoder;
    encoder.Attach(new CryptoPP::StringSink(hexDigest));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();
    
    for(auto & c: hexDigest) c = toupper(c);
    return hexDigest;
}

bool Authenticator::verify(const std::string& salt16, const std::string& password, const std::string& receivedHash) {
    std::string computedHash = computeHash(salt16, password);
    return computedHash == receivedHash;
}

void Authenticator::parseAuthMessage(const std::string& msg, std::string& login, std::string& salt, std::string& hash) {
    if (msg.length() < 72) {
        throw AuthError("Сообщение аутентификации слишком короткое");
    }
    size_t suffixLen = 16 + 56;
    login = msg.substr(0, msg.length() - suffixLen);
    salt = msg.substr(login.length(), 16);
    hash = msg.substr(login.length() + 16, 56);
}