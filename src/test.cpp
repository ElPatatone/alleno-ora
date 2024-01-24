#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main (int argc, char *argv[]) {
    std::vector<std::string_view> args(argv + 1, argv + argc);
    // for (const auto& arg : args) {
    //     std::cout << arg << "\n";
    //     std::cout << arg << "\n";
    // }
    for (int i = 0; i < args.size(); i++) {
        // if (args[i] == "-s" && args[i + 1] == "-s") {
        //     std::cout << "cannot use the same cmd twice\n";
        // }
        if (args[i] == args[i + 1]) {
            std::cout << "cannot use the same cmd twice\n";
            return 1;
        }
        if (args[i] == "-s") {
            std::ifstream file{std::string(args[i + 1])};
            if (!file.is_open()) {
                std::cout << "there was an error with opening the file " << args[i + 1] << "\n";
                return 1;
            }
            else {
                std::cout << "the file was opened properly " << args[i + 1] << "\n";
            }
        }
        if (args[i] == "-c") {
            if (args[i + 1] == "") {
                std::cout << "Cannot pass nothing to this command" << "\n";
            }
        }
    }
    return 0;
}
