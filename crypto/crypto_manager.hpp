#pragma once
#include <string>

class CryptoManager {
public:
    static std::string encrypt(const std::string_view plaintext,
                               const std::string& password);

    static std::string decrypt(const std::string_view ciphertext,
                               const std::string& password);
};