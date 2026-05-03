#pragma once
#include <string>

class CryptoManager {
public:
    static std::string encrypt(const std::string& plaintext,
                               const std::string& password);

    static std::string decrypt(const std::string& ciphertext,
                               const std::string& password);
};