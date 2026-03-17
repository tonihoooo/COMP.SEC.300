#include <iostream>
#include <string>

void handle_init();
void handle_add(const std::string& name);
void handle_get(const std::string& name);
void handle_list();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: passman <command>\n";
        return 1;
    }

    std::string command = argv[1];

    if (command == "init") {
        handle_init();
    } else if (command == "add") {
        if (argc < 3) {
            std::cerr << "Error: Missing entry name\n";
            return 1;
        }
        handle_add(argv[2]);
    } else if (command == "get") {
        if (argc < 3) {
            std::cerr << "Error: Missing entry name\n";
            return 1;
        }
        handle_get(argv[2]);
    } else if (command == "list") {
        handle_list();
    } else {
        std::cerr << "Unknown command\n";
        return 1;
    }

    return 0;
}