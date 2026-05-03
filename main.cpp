#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include "storage/file_storage.hpp"

// XOR encryption/decryption
std::string xor_encrypt_decrypt(const std::string& data, const std::string& key) {
    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] ^= key[i % key.size()]; 
    }
    return result;
}

int main() {
    const std::string filename = "vault.dat";

    std::string master_password;
    std::cout << "Enter master password: ";
    std::cin >> master_password;

    // Load vault
    std::vector<Entry> vault = FileStorage::load(filename, master_password);
    std::cout << "\nVault loaded (" << vault.size() << " entries).\n";

    std::string command;

    while (true) {
        std::cout << "\nCommands: add | list | exit\n> ";
        std::cin >> command;

        if (command == "add") {
            Entry e;
            std::cout << "Entry name: "; std::cin >> e.name;
            std::cout << "Username: "; std::cin >> e.username;
            std::cout << "Password: "; std::cin >> e.password;
            vault.push_back(e);
            FileStorage::save(filename, vault, master_password);
            std::cout << "Entry added and saved.\n";

        } else if (command == "list") {
            if (vault.empty()) std::cout << "Vault is empty.\n";
            else {
                std::cout << "Vault entries:\n";
                for (auto& e : vault) {
                    std::cout << "- " << e.name << " | " << e.username << " | " << e.password << "\n";
                }
            }

        } else if (command == "exit") {
            std::cout << "Exiting program.\n";
            break;

        } else {
            std::cout << "Unknown command. Use: add | list | exit\n";
        }
    }

    return 0;
}