#include "../crypto/crypto_manager.hpp"
#include "file_storage.hpp"

#include <fstream>
#include <cstdint>
#include <iostream>

static const std::string MAGIC = "MYMGR";
static const uint8_t VERSION = 1;

/**
 * Serializes the vault entries into a binary format. 
 * The format is: [entry_count][entry1_name_len][entry1_name][entry1_username_len][entry1_username][entry1_password_len][entry1_password]...
 * 
 * @param vault The vector of entries to serialize.
 * @return A string containing the serialized vault data.
 */
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

/**
 * Deserializes the vault data from the binary format produced by serialize_vault.
 * 
 * @param buffer The string containing the serialized vault data.
 * @return A vector of entries reconstructed from the serialized data.
 */
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

/**
 * Saves the vault to a file. The vault is serialized and encrypted using the master password before being written to disk. 
 * The file format includes a header with a magic string and version number for validation.
 * 
 * @param filename The name of the file to save the vault to.
 * @param vault The vector of entries to save.
 * @param master_password The password to encrypt the vault with.
 */
void FileStorage::save(const std::string& filename,
                       const std::vector<Entry>& vault,
                       const std::string& master_password) {

    std::ofstream ofs(filename, std::ios::binary);

    std::string serialized = serialize_vault(vault);
    std::string encrypted = CryptoManager::encrypt(serialized, master_password);

    ofs.write(MAGIC.c_str(), MAGIC.size());
    ofs.write(reinterpret_cast<const char*>(&VERSION), sizeof(VERSION));
    ofs.write(encrypted.data(), encrypted.size());
}

/**
 * Loads the vault from a file. The file is read and decrypted using the master password, then deserialized into a vector of entries.
 * 
 * @param filename The name of the file to load the vault from.
 * @param master_password The password to decrypt the vault with.
 * @return A vector of entries reconstructed from the loaded and decrypted data.
 */
std::vector<Entry> FileStorage::load(const std::string& filename,
                                     const std::string& master_password) {

    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) return {};

    std::string magic(MAGIC.size(), '\0');
    ifs.read(&magic[0], MAGIC.size());

    if (magic != MAGIC) {
        throw std::runtime_error("Invalid vault file (magic mismatch)");
    }

    uint8_t version;
    ifs.read(reinterpret_cast<char*>(&version), sizeof(version));

    if (version != VERSION) {
        throw std::runtime_error("Unsupported vault version");
    }

    std::string encrypted((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());

    std::string decrypted = CryptoManager::decrypt(encrypted, master_password);

    return deserialize_vault(decrypted);
}