#pragma once

#include <string>
#include <optional>
#include <vector>
#include "entry.hpp"

class VaultManager {
public:
    bool init(const std::string& path);
    bool load(const std::string& path);
    bool save();

    bool add_entry(const Entry& entry);
    std::optional<Entry> get_entry(const std::string& name);
    std::vector<Entry> list_entries() const;

private:
    std::string vault_path;
    std::vector<Entry> entries;
    bool is_loaded = false;
};