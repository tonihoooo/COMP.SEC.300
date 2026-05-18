#include "catch.hpp"
#include "../crypto/crypto_manager.hpp"

/**
 * Tests the encryption and decryption functionality of the CryptoManager class. 
 */
TEST_CASE("Crypto encrypt/decrypt works", "[crypto]") {
    std::string data = "hello world";
    std::string key = "secret";

    auto encrypted = CryptoManager::encrypt(data, key);
    auto decrypted = CryptoManager::decrypt(encrypted, key);

    REQUIRE(decrypted == data);
}