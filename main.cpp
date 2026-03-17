#include <iostream>
#include <string>
#include <vector>

struct Entry {
    std::string name;
    std::string username;
    std::string password;
};

int main() {
    std::string master_password;
    std::cout << "Enter master password: ";
    std::cin >> master_password;

    std::vector<Entry> vault;
    std::string command;

    std::cout << "\nVault ready (local file: vault.dat)\n";

    while (true) {
        std::cout << "\nCommands: add | list | exit\n> ";
        std::cin >> command;

        if (command == "add") {
            Entry e;
            std::cout << "Entry name: "; std::cin >> e.name;
            std::cout << "Username: "; std::cin >> e.username;
            std::cout << "Password: "; std::cin >> e.password;
            vault.push_back(e);
            std::cout << "Entry added.\n";

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