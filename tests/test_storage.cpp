#include "catch.hpp"
#include "../storage/file_storage.hpp"

TEST_CASE("Save and load vault", "[storage]") {
    std::vector<Entry> vault = {
        {"github", "user", "pass"}
    };

    FileStorage::save("test_vault.dat", vault, "secret");
    auto loaded = FileStorage::load("test_vault.dat", "secret");

    REQUIRE(loaded.size() == 1);
    REQUIRE(loaded[0].name == "github");
    REQUIRE(loaded[0].username == "user");
    REQUIRE(loaded[0].password == "pass");
}