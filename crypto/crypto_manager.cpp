#include "crypto_manager.hpp"

std::string CryptoManager::xor_encrypt_decrypt(const std::string& data,
                                               const std::string& key) {
    std::string result = data;

    for (size_t i = 0; i < data.size(); ++i) {
        result[i] ^= key[i % key.size()];
    }

    return result;
}