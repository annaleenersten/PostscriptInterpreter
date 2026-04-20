#include "interpreter.h"
#include <iostream>
#include <sstream>


/*
 * Prints the current operand stack (OP_STACK).
 * Each value is printed in order from bottom to top.
 */
void print_op_stack() {
    std::cout << "OP_STACK = [ ";

    for (const auto& v : op_stack) {
        print_value(v);
        std::cout << " ";
    }

    std::cout << "]\n";
}

/*
 * Prints the entire dictionary stack.
 * Each dictionary level is shown with:
 *  - its level index
 *  - whether it is the top of the stack
 *  - its stored keys
 *  - whether it has a parent dictionary
 */

void print_dict_stack() {
    std::cout << "\n===== DICT STACK =====\n";

    for (int i = 0; i < (int)dict_stack.size(); i++) {
        PSDict* d = dict_stack[i];

        std::cout << "Level " << i;

        if (i == (int)dict_stack.size() - 1)
            std::cout << " (TOP)";
        std::cout << "\n";

        std::cout << "Keys: { ";

        for (const auto& [key, val] : d->dict) {
            std::cout << key << " ";
        }

        std::cout << "}\n";

        if (d->parent) {
            std::cout << "Parent exists\n";
        } else {
            std::cout << "Parent: nullptr\n";
        }

        std::cout << "----------------------\n";
    }

    std::cout << "======================\n\n";
}

/*
 * Tokenizes an input string into interpreter tokens.
 *
 * Rules:
 *  - Whitespace separates tokens outside of code blocks
 *  - Everything inside { ... } is treated as a single token
 *  - Nested braces are supported using brace_depth tracking
 *
 * Returns:
 *  Vector of parsed string tokens
 */

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

/*
 * Read–Eval–Print Loop (REPL) for the interpreter.
 *
 * Behavior:
 *  - Reads user input line by line
 *  - Supports multi-line code blocks using { ... }
 *  - Buffers input until all braces are closed
 *  - Tokenizes and processes input once complete
 *  - Supports "quit" command to exit
 *  - Prints operand stack after each evaluation
 */
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
        //print_dict_stack();
    }
}

/*
 * Program entry point.
 * Initializes interpreter state and starts the REPL.
 */
int main() {
    init_interpreter();
    repl();
}