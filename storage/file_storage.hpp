#pragma once
#include <string>
#include <vector>

/**
 * Represents a single entry in the password vault, containing a name, username, and password.
 */
struct Entry {
    std::string name;
    std::string username;
    std::string password;
};

/**
 * Provides file-based storage for the password vault.
 */
class FileStorage {
public:
    static void save(const std::string& filename,
                     const std::vector<Entry>& vault,
                     const std::string& master_password);

    static std::vector<Entry> load(const std::string& filename,
                                   const std::string& master_password);
};