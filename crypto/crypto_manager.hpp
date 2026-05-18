#pragma once
#include <string>

/**
 * CryptoManager provides static methods for encrypting and decrypting data using a password. 
 */
class CryptoManager {
public:
    static std::string encrypt(const std::string_view plaintext,
                               const std::string& password);

    static std::string decrypt(const std::string_view ciphertext,
                               const std::string& password);
};