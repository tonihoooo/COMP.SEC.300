#pragma once
#include <string>

class CryptoManager {
public:
    static std::string xor_encrypt_decrypt(const std::string& data,
                                           const std::string& key);
};