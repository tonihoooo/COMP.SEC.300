#include "app/vault_manager.hpp"

#include <iostream>
#include <string>
#include <sodium.h>

int main() {
    if (sodium_init() < 0) {
        std::cerr << "Failed to initialize libsodium\n";
        return 1;
    }

    const std::string filename = "vault.dat";
    std::vector<char> master_password;

    VaultManager vm;

    while (true) {
        std::cout << "Enter master password: ";
        std::string temp;
        std::cin >> temp;

        master_password.assign(temp.begin(), temp.end());
        std::fill(temp.begin(), temp.end(), '\0');
        std::string password_str(master_password.begin(), master_password.end());

        if (vm.load(filename, password_str)) {
            break;
        }

        std::cout << "Wrong password or corrupted vault. Try again.\n";
    }

    std::cout << "\nVault loaded successfully.\n";

    std::string command;

    while (true) {
        std::cout << "\nCommands: add | list | delete | exit\n> ";
        std::cin >> command;

        if (command == "add") {
            Entry e;
            std::cout << "Entry name: "; std::cin >> e.name;
            std::cout << "Username: "; std::cin >> e.username;
            std::cout << "Password: "; std::cin >> e.password;

            vm.add_entry(e);
            vm.save();

            std::cout << "Entry added and saved.\n";

        } else if (command == "list") {
            auto entries = vm.list_entries();

            if (entries.empty()) {
                std::cout << "Vault is empty.\n";
            } else {
                std::cout << "Vault entries:\n";
                for (const auto& e : entries) {
                    std::cout << "- " << e.name
                              << " | " << e.username
                              << " | " << e.password << "\n";
                }
            }

        } else if (command == "exit") {
            std::cout << "Exiting program.\n";
            break;

        } else if (command == "delete") {
            std::string name;
            std::cout << "Entry name to delete: ";
            std::cin >> name;

            if (vm.delete_entry(name)) {
                vm.save();
                std::cout << "Entry deleted.\n";
            } else {
                std::cout << "Entry not found.\n";
            }
        } else {
            std::cout << "Unknown command. Use: add | list | delete | exit\n";
        }
    }

    std::fill(master_password.begin(), master_password.end(), '\0');
    return 0;
}