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

std::vector<std::string> tokenize(const std::string& input) {
    std::vector<std::string> tokens;

    std::string current;
    bool in_block = false;
    int brace_depth = 0;

    for (size_t i = 0; i < input.size(); i++) {
        char c = input[i];

        // Start of code block
        if (c == '{') {
            in_block = true;
            brace_depth++;
            current += c;
            continue;
        }

        // End of code block
        if (c == '}') {
            current += c;
            brace_depth--;

            if (brace_depth == 0) {
                tokens.push_back(current);
                current.clear();
                in_block = false;
            }
            continue;
        }

        // Inside a block
        if (in_block) {
            current += c;
            continue;
        }

        // Outside block → normal whitespace tokenization
        if (std::isspace(c)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        }
        else {
            current += c;
        }
    }

    if (!current.empty()) {
        tokens.push_back(current);
    }

    return tokens;
}

void repl() {
    std::string input;
    std::string buffer;
    int brace_depth = 0;

    while (true) {
        std::cout << "REPL> ";
        std::getline(std::cin, input);

        buffer += input + " ";

        for (char c : input) {
            if (c == '{') brace_depth++;
            if (c == '}') brace_depth--;
        }

        // Only process when full block is complete OR no block
        if (brace_depth > 0) {
            continue; // keep collecting lines
        }

        try {
            std::vector<std::string> tokens = tokenize(buffer);

            for (const auto& token : tokens) {
                if (token == "quit") return;
                process_input(token);
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }

        buffer.clear();
        print_op_stack();
    }
}

int main() {
    init_interpreter();
    repl();
}