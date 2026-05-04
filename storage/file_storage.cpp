#include "../crypto/crypto_manager.hpp"
#include "file_storage.hpp"

#include <fstream>
#include <cstdint>
#include <iostream>

static std::string serialize_vault(const std::vector<Entry>& vault) {
    std::string buffer;

    auto write_string = [&buffer](const std::string& s) {
        uint32_t len = s.size();
        buffer.append(reinterpret_cast<const char*>(&len), sizeof(len));
        buffer.append(s);
    };

    uint32_t count = vault.size();
    buffer.append(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& e : vault) {
        write_string(e.name);
        write_string(e.username);
        write_string(e.password);
    }

    return buffer;
}

static std::vector<Entry> deserialize_vault(const std::string& buffer) {
    std::vector<Entry> vault;
    size_t offset = 0;

    auto read_string = [&buffer, &offset]() {
        uint32_t len;
        memcpy(&len, buffer.data() + offset, sizeof(len));
        offset += sizeof(len);

        std::string s(buffer.data() + offset, len);
        offset += len;

        return s;
    };

    uint32_t count;
    memcpy(&count, buffer.data(), sizeof(count));
    offset += sizeof(count);

    for (uint32_t i = 0; i < count; ++i) {
        Entry e;
        e.name = read_string();
        e.username = read_string();
        e.password = read_string();
        vault.push_back(e);
    }

    return vault;
}

void FileStorage::save(const std::string& filename,
                       const std::vector<Entry>& vault,
                       const std::string& master_password) {

    std::ofstream ofs(filename, std::ios::binary);

    std::string serialized = serialize_vault(vault);
    std::string encrypted = CryptoManager::encrypt(serialized, master_password);

    ofs.write(encrypted.data(), encrypted.size());
}

std::vector<Entry> FileStorage::load(const std::string& filename,
                                     const std::string& master_password) {

    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) return {};

    std::string encrypted((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());

    try {
        std::string decrypted = CryptoManager::decrypt(encrypted, master_password);
        return deserialize_vault(decrypted);
    } catch (...) {
        std::cerr << "Failed to decrypt vault (wrong password or corrupted file)\n";
        return {};
    }
}