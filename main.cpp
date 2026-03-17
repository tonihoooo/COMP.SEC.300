#include <iostream>
#include <string>
#include "app/vault_manager.hpp"

void print_usage() {
    std::cout << "Usage:\n";
    std::cout << "  passman init <vault_file>\n";
    std::cout << "  passman add <vault_file> <entry_name>\n";
    std::cout << "  passman get <vault_file> <entry_name>\n";
    std::cout << "  passman list <vault_file>\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    std::string command = argv[1];
    std::string vault_file = argv[2];

    VaultManager vm;
    std::string master_password;

    std::cout << "Master password: ";
    std::cin >> master_password;

    if (command == "init") {
        if (!vm.init(vault_file, master_password)) {
            std::cerr << "Failed to initialize vault\n";
            return 1;
        }
        std::cout << "Vault initialized successfully.\n";

    } else if (command == "add") {
        if (argc < 4) {
            std::cerr << "Missing entry name\n";
            return 1;
        }
        std::string entry_name = argv[3];

        if (!vm.load(vault_file, master_password)) return 1;

        Entry e;
        e.name = entry_name;

        std::cout << "Username: ";
        std::cin >> e.username;
        std::cout << "Password: ";
        std::cin >> e.password;

        vm.add_entry(e);
        vm.save();

        std::cout << "Entry added.\n";

    } else if (command == "get") {
        if (argc < 4) {
            std::cerr << "Missing entry name\n";
            return 1;
        }
        std::string entry_name = argv[3];

        if (!vm.load(vault_file, master_password)) return 1;

        auto entry = vm.get_entry(entry_name);
        if (entry) {
            std::cout << "Username: " << entry->username << "\n";
            std::cout << "Password: " << entry->password << "\n";
        } else {
            std::cerr << "Entry not found\n";
        }

    } else if (command == "list") {
        if (!vm.load(vault_file, master_password)) return 1;

        auto list = vm.list_entries();
        std::cout << "Entries:\n";
        for (const auto& e : list) {
            std::cout << "  - " << e.name << "\n";
        }

    } else {
        print_usage();
        return 1;
    }

    return 0;
}