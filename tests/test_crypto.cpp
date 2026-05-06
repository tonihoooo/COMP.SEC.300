#include "catch.hpp"
#include "../crypto/crypto_manager.hpp"

TEST_CASE("Crypto encrypt/decrypt works", "[crypto]") {
    std::string data = "hello world";
    std::string key = "secret";

    auto encrypted = CryptoManager::encrypt(data, key);
    auto decrypted = CryptoManager::decrypt(encrypted, key);

    REQUIRE(decrypted == data);
}