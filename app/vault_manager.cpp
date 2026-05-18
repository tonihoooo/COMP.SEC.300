#include "vault_manager.hpp"
#include <algorithm>

/**
 * Loads the vault from the specified file using the provided master password. 
 * 
 * @filename The name of the file to load the vault from.
 * @master_password The master password to decrypt the vault.
 * @return true if the vault was loaded successfully, false otherwise.
 */
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

/**
 * Saves the current vault to the file specified during loading. The vault is encrypted using the master password.
 */
void VaultManager::save() {
    FileStorage::save(filename, vault, master_password);
}

/**
 * Adds a new entry to the vault. The entry is not saved to disk until save() is called.
 */
void VaultManager::add_entry(const Entry& entry) {
    vault.push_back(entry);
}

/**
 * Lists all entries in the vault.
 * 
 * @return A vector containing all entries in the vault.
 */
std::vector<Entry> VaultManager::list_entries() const {
    return vault;
}

/**
 * Deletes an entry from the vault by name.
 * 
 * @param name The name of the entry to delete.
 * @return true if the entry was deleted, false if not found.
 */
bool VaultManager::delete_entry(const std::string& name) {
    auto it = std::remove_if(vault.begin(), vault.end(),
        [&](const Entry& e) {
            return e.name == name;
        });

    if (it == vault.end()) {
        return false;
    }

    vault.erase(it, vault.end());
    return true;
}