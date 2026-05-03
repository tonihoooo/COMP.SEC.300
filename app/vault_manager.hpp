#pragma once
#include <vector>
#include <string>
#include <optional>
#include "../storage/file_storage.hpp"

class VaultManager {
public:
    bool load(const std::string& filename, const std::string& master_password);
    void save();

    void add_entry(const Entry& entry);
    std::vector<Entry> list_entries() const;

private:
    std::string filename;
    std::string master_password;
    std::vector<Entry> vault;
};