#pragma once
#include <string>
#include <vector>

struct Entry {
    std::string name;
    std::string username;
    std::string password;
};

class FileStorage {
public:
    static void save(const std::string& filename,
                     const std::vector<Entry>& vault,
                     const std::string& master_password);

    static std::vector<Entry> load(const std::string& filename,
                                   const std::string& master_password);
};