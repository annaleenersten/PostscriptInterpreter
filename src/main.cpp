#include "interpreter.h"
#include <iostream>
#include <sstream>

bool STATIC_SCOPING = false;

void print_op_stack() {
    std::cout << "OP_STACK = [ ";

    for (const auto& v : op_stack) {
        print_value(v);
        std::cout << " ";
    }

    std::cout << "]\n";
}

void repl() {
    std::string input;

    while (true) {
        std::cout << "REPL> ";
        std::getline(std::cin, input);

        try {
            std::stringstream ss(input);
            std::string token;

            while (ss >> token) {
                if (token == "quit") return;
                process_input(token);
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