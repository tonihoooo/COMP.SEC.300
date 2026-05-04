#include "vault_manager.hpp"
#include <algorithm>

bool VaultManager::load(const std::string& filename, const std::string& master_password) {
    this->filename = filename;
    this->master_password = master_password;

    try {
        vault = FileStorage::load(filename, master_password);
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

void VaultManager::save() {
    FileStorage::save(filename, vault, master_password);
}

void VaultManager::add_entry(const Entry& entry) {
    vault.push_back(entry);
}

std::vector<Entry> VaultManager::list_entries() const {
    return vault;
}

bool VaultManager::delete_entry(const std::string& name) {
    auto it = std::remove_if(vault.begin(), vault.end(),
        [&](const Entry& e) {
            return e.name == name;
        });

    if (it == vault.end()) {
        return false; // not found
    }

    vault.erase(it, vault.end());
    return true;
}