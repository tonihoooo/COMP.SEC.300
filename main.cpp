#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

struct Entry {
    std::string name;
    std::string username;
    std::string password;
};

// XOR encryption/decryption
std::string xor_encrypt_decrypt(const std::string& data, const std::string& key) {
    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] ^= key[i % key.size()]; 
    }
    return result;
}

// Save vault to file (with XOR)
void save_vault(const std::string& filename, const std::vector<Entry>& vault, const std::string& master_password) {
    std::ofstream ofs(filename, std::ios::binary);
    for (const auto& e : vault) {
        auto write_string = [&ofs, &master_password](const std::string& s) {
            std::string encrypted = xor_encrypt_decrypt(s, master_password);
            uint32_t len = encrypted.size();
            ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
            ofs.write(encrypted.data(), len);
        };
        write_string(e.name);
        write_string(e.username);
        write_string(e.password);
    }
}

// Load vault from file (with XOR)
std::vector<Entry> load_vault(const std::string& filename, const std::string& master_password) {
    std::vector<Entry> vault;
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) return vault;

    while (ifs.peek() != EOF) {
        auto read_string = [&ifs, &master_password]() -> std::string {
            uint32_t len;
            ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
            std::string s(len, '\0');
            ifs.read(&s[0], len);
            return xor_encrypt_decrypt(s, master_password);
        };
        Entry e;
        e.name = read_string();
        e.username = read_string();
        e.password = read_string();
        vault.push_back(e);
    }
    return vault;
}

int main() {
    const std::string filename = "vault.dat";

    std::string master_password;
    std::cout << "Enter master password: ";
    std::cin >> master_password;

    // Load vault
    std::vector<Entry> vault = load_vault(filename, master_password);
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
            save_vault(filename, vault, master_password);
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