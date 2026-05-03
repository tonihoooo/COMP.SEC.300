#include "vault_manager.hpp"

bool VaultManager::load(const std::string& filename, const std::string& master_password) {
    this->filename = filename;
    this->master_password = master_password;

    vault = FileStorage::load(filename, master_password);
    return true;
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