#include "file_storage.hpp"
#include <fstream>
#include <cstdint>

// XOR encryption/decryption (temporary)
static std::string xor_encrypt_decrypt(const std::string& data, const std::string& key) {
    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] ^= key[i % key.size()];
    }
    return result;
}

void FileStorage::save(const std::string& filename,
                       const std::vector<Entry>& vault,
                       const std::string& master_password) {

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

std::vector<Entry> FileStorage::load(const std::string& filename,
                                     const std::string& master_password) {

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