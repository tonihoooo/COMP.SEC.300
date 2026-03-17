#pragma once
#include <string>
#include <vector>
#include <optional>
#include <cstdint>
#include "../crypto/crypto_manager.hpp"
#include "../storage/file_storage.hpp"

struct Entry {
    std::string name;
    std::string username;
    std::string password;
};

class VaultManager {
public:
    // Initialize a new vault file
    bool init(const std::string& path, const std::string& master_password);

    // Load an existing vault
    bool load(const std::string& path, const std::string& master_password);

    // Save current vault
    bool save();

    // Entry management
    bool add_entry(const Entry& entry);
    std::optional<Entry> get_entry(const std::string& name);
    std::vector<Entry> list_entries() const;

private:
    std::string vault_path;
    std::vector<Entry> entries;
    std::vector<unsigned char> key;
    std::vector<unsigned char> salt;
    std::vector<unsigned char> nonce;
    bool is_loaded = false;

    // Binary serialization / deserialization
    std::string serialize_entries() const;
    void deserialize_entries(const std::string& data);
};