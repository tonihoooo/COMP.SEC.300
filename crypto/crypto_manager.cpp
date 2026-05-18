#include "crypto_manager.hpp"
#include <sodium.h>
#include <vector>
#include <stdexcept>

static const size_t SALT_SIZE = crypto_pwhash_SALTBYTES;

/**
 * Encrypts the given plaintext using the provided password. The output format is: [salt][nonce][ciphertext].
 * 
 * @param plaintext The plaintext to encrypt.
 * @param password The password to derive the encryption key from.
 * @return The encrypted data as a string.
 * @throws std::runtime_error if key derivation or encryption fails.
 */
std::string CryptoManager::encrypt(const std::string_view plaintext,
                                   const std::string& password) {
    unsigned char salt[SALT_SIZE];
    randombytes_buf(salt, sizeof salt);

    std::string key(crypto_secretbox_KEYBYTES, '\0');
    if (crypto_pwhash((unsigned char*)key.data(), key.size(),
                      password.c_str(), password.size(),
                      salt,
                      crypto_pwhash_OPSLIMIT_INTERACTIVE,
                      crypto_pwhash_MEMLIMIT_INTERACTIVE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        throw std::runtime_error("Key derivation failed");
    }

    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, sizeof nonce);

    std::vector<unsigned char> ciphertext(plaintext.size() + crypto_secretbox_MACBYTES);

    crypto_secretbox_easy(ciphertext.data(),
                          (const unsigned char*)plaintext.data(),
                          plaintext.size(),
                          nonce,
                          (const unsigned char*)key.data());

    std::string result;
    result.append((char*)salt, sizeof salt);
    result.append((char*)nonce, sizeof nonce);
    result.append((char*)ciphertext.data(), ciphertext.size());

    return result;
}

/**
 * Decrypts the given ciphertext using the provided password. The input format is expected to be: [salt][nonce][ciphertext].
 * 
 * @param data The encrypted data to decrypt.
 * @param password The password to derive the decryption key from.
 * @return The decrypted plaintext as a string.
 * @throws std::runtime_error if key derivation or decryption fails.
 */
std::string CryptoManager::decrypt(const std::string_view data,
                                   const std::string& password) {

    if (data.size() < SALT_SIZE + crypto_secretbox_NONCEBYTES)
        throw std::runtime_error("Invalid data");

    const unsigned char* salt = (const unsigned char*)data.data();
    const unsigned char* nonce = (const unsigned char*)(data.data() + SALT_SIZE);
    const unsigned char* ciphertext = (const unsigned char*)(data.data() + SALT_SIZE + crypto_secretbox_NONCEBYTES);

    size_t ciphertext_len = data.size() - SALT_SIZE - crypto_secretbox_NONCEBYTES;

    std::string key(crypto_secretbox_KEYBYTES, '\0');
    if (crypto_pwhash((unsigned char*)key.data(), key.size(),
                      password.c_str(), password.size(),
                      salt,
                      crypto_pwhash_OPSLIMIT_INTERACTIVE,
                      crypto_pwhash_MEMLIMIT_INTERACTIVE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        throw std::runtime_error("Key derivation failed");
    }

    std::vector<unsigned char> plaintext(ciphertext_len - crypto_secretbox_MACBYTES);

    if (crypto_secretbox_open_easy(plaintext.data(),
                                  ciphertext,
                                  ciphertext_len,
                                  nonce,
                                  (const unsigned char*)key.data()) != 0) {
        throw std::runtime_error("AUTHENTICATION_FAILED");
    }

    return std::string((char*)plaintext.data(), plaintext.size());
}