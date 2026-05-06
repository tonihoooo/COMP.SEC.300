#include "catch.hpp"
#include "../app/vault_manager.hpp"

TEST_CASE("Vault basic operations", "[vault]") {
    VaultManager vm;

    REQUIRE(vm.load("test_vault.dat", "secret"));

    Entry e{"site", "user", "pass"};
    vm.add_entry(e);

    REQUIRE(vm.list_entries().size() >= 1);

    REQUIRE(vm.delete_entry("site") == true);
}