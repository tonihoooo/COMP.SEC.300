#include "vault_manager.hpp"
#include <fstream>
#include <iostream>

bool VaultManager::init(const std::string& path) {
    vault_path = path;
    entries.clear();
    is_loaded = true;
    return save();
}

bool VaultManager::load(const std::string& path) {
    vault_path = path;
    entries.clear();

    std::ifstream file(path);
    if (!file) return false;

    std::string name, username, password;

    while (file >> name >> username >> password) {
        entries.push_back({name, username, password});
    }

    is_loaded = true;
    return true;
}

bool VaultManager::save() {
    if (!is_loaded) return false;

    std::ofstream file(vault_path);
    if (!file) return false;

    for (const auto& e : entries) {
        file << e.name << " " << e.username << " " << e.password << "\n";
    }

    return true;
}

bool VaultManager::add_entry(const Entry& entry) {
    if (!is_loaded) return false;

    entries.push_back(entry);
    return true;
}

std::optional<Entry> VaultManager::get_entry(const std::string& name) {
    for (const auto& e : entries) {
        if (e.name == name) return e;
    }
    return std::nullopt;
}

std::vector<Entry> VaultManager::list_entries() const {
    return entries;
}