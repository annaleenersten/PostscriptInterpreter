#include "interpreter.h"
#include "operations.h"
#include <iostream>
#include <functional>
#include <stdexcept>

std::vector<Value> op_stack;
std::vector<PSDict> dict_stack;

void init_interpreter() {
    dict_stack.push_back(PSDict());

    dict_stack.back().dict["add"] = std::function<void()>(add_operation);
    dict_stack.back().dict["mul"] = std::function<void()>(mul_operation);
    dict_stack.back().dict["="] = std::function<void()>(pop_print_operation);
    dict_stack.back().dict["def"] = std::function<void()>(def_operation);
}

void lookup_in_dictionary(const std::string& input) {
    for (auto it = dict_stack.rbegin(); it != dict_stack.rend(); ++it) {
        PSDict& current = *it;

        if (current.contains(input)) {
            Value value = current.get(input);

            // CASE 1: function
            if (std::holds_alternative<std::function<void()>>(value)) {
                std::get<std::function<void()>>(value)();
            }
            // CASE 2: code block
            else if (std::holds_alternative<std::vector<std::string>>(value)) {
                auto code = std::get<std::vector<std::string>>(value);
                for (const auto& token : code) {
                    process_input(token);
                }
            }
            // CASE 3: literal value
            else {
                op_stack.push_back(value);
            }

            return;
        }
    }

    throw ParseFailed("Could not find " + input + " in dictionary");
}

// void process_input(const std::string& input) {
//     try {
//         // Try parsing literal (number, bool, string, code block)
//         Value v = process_constants(input);
//         op_stack.push_back(v);
//     }
//     catch (ParseFailed&) {
//         // Not a literal → must be dictionary lookup
//         try {
//             lookup_in_dictionary(input);
//         }
//         catch (const std::exception& e) {
//             // std::cerr << "Error: " << e.what() << std::endl;
//             // throw;
//             op_stack.push_back(input);
//         }
//     }
// }

void process_input(const std::string& input) {
    try {
        Value v = process_constants(input);
        op_stack.push_back(v);
        return;
    }
    catch (ParseFailed& e) {
        // not a literal → must be operator or variable
        // throw TypeMismatch("Undefined token: " + input);
    }

    lookup_in_dictionary(input);
}