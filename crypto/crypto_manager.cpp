#include "crypto_manager.hpp"
#include <sodium.h>
#include <vector>
#include <stdexcept>

static const size_t SALT_SIZE = crypto_pwhash_SALTBYTES;

std::string CryptoManager::encrypt(const std::string_view plaintext,
                                   const std::string& password) {
    // Generate random salt
    unsigned char salt[SALT_SIZE];
    randombytes_buf(salt, sizeof salt);

    // Derive key from password
    unsigned char key[crypto_secretbox_KEYBYTES];
    if (crypto_pwhash(key, sizeof key,
                      password.c_str(), password.size(),
                      salt,
                      crypto_pwhash_OPSLIMIT_INTERACTIVE,
                      crypto_pwhash_MEMLIMIT_INTERACTIVE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        throw std::runtime_error("Key derivation failed");
    }

    // Generate random nonce
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, sizeof nonce);

    std::vector<unsigned char> ciphertext(plaintext.size() + crypto_secretbox_MACBYTES);

    // Encrypt the plaintext
    crypto_secretbox_easy(ciphertext.data(),
                          (const unsigned char*)plaintext.data(),
                          plaintext.size(),
                          nonce,
                          key);

    std::string result;
    result.append((char*)salt, sizeof salt);
    result.append((char*)nonce, sizeof nonce);
    result.append((char*)ciphertext.data(), ciphertext.size());

    return result;
}

std::string CryptoManager::decrypt(const std::string_view data,
                                   const std::string& password) {

    // Validate input size
    if (data.size() < SALT_SIZE + crypto_secretbox_NONCEBYTES)
        throw std::runtime_error("Invalid data");

    // Extract salt, nonce, and ciphertext
    const unsigned char* salt = (const unsigned char*)data.data();
    const unsigned char* nonce = (const unsigned char*)(data.data() + SALT_SIZE);
    const unsigned char* ciphertext = (const unsigned char*)(data.data() + SALT_SIZE + crypto_secretbox_NONCEBYTES);

    // Calculate ciphertext length
    size_t ciphertext_len = data.size() - SALT_SIZE - crypto_secretbox_NONCEBYTES;

    // Derive key from password
    unsigned char key[crypto_secretbox_KEYBYTES];
    if (crypto_pwhash(key, sizeof key,
                      password.c_str(), password.size(),
                      salt,
                      crypto_pwhash_OPSLIMIT_INTERACTIVE,
                      crypto_pwhash_MEMLIMIT_INTERACTIVE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        throw std::runtime_error("Key derivation failed");
    }

    // Decrypt the ciphertext
    std::vector<unsigned char> plaintext(ciphertext_len - crypto_secretbox_MACBYTES);

    if (crypto_secretbox_open_easy(plaintext.data(),
                                  ciphertext,
                                  ciphertext_len,
                                  nonce,
                                  key) != 0) {
        throw std::runtime_error("AUTHENTICATION_FAILED");
    }

    return std::string((char*)plaintext.data(), plaintext.size());
}