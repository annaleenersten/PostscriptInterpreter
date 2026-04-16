#include "interpreter.h"
#include <iostream>
#include <sstream>

void print_op_stack() {
    std::cout << "OP_STACK = [ ";

    for (const auto& v : op_stack) {
        print_value(v);
        std::cout << " ";
    }

    std::cout << "]\n";
}

// void repl() {
//     std::string input;

//     while (true) {
//         std::cout << "REPL> ";
//         std::getline(std::cin, input);

//         if (!input.empty() && input[0] == '{') {
//             process_input(input);
//         } else {
//             std::stringstream ss(input);
//             std::string token;

//             while (ss >> token) {
//                 if (token == "quit") return;
//                 process_input(token);
//             }
//         }

//         print_op_stack();
//     }
// }

void repl() {
    std::string input;

    while (true) {
        std::cout << "REPL> ";
        std::getline(std::cin, input);

        try {
            if (!input.empty() && input[0] == '{') {
                process_input(input);
            } else {
                std::stringstream ss(input);
                std::string token;

                while (ss >> token) {
                    if (token == "quit") return;
                    process_input(token);
                }
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        print_op_stack();
    }
}

int main() {
    init_interpreter();
    repl();
}