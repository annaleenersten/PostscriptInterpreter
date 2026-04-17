#include "parser.h"

#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <value.h>


// ---------- CUSTOM EXCEPTIONS ----------
ParseFailed::ParseFailed(const std::string& msg) : message(msg) {}

const char* ParseFailed::what() const noexcept {
    return message.c_str();
}

TypeMismatch::TypeMismatch(const std::string& msg) : message(msg) {}

const char* TypeMismatch::what() const noexcept {
    return message.c_str();
}


// ---------- HELPERS ----------
std::string to_lower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// ---------- PARSERS ----------

// Boolean parser
Value process_boolean(const std::string& input) {
    if (input == "true"){
        return true;
    }
    else if (input == "false"){
        return false;
    } 
    else{
        throw ParseFailed("can't parse input into boolean");
    }
}

// Number parser
Value process_number(const std::string& input) {
    try {
        double value = std::stod(input);

        if (value == static_cast<int>(value)) {
            return static_cast<int>(value);
        }
        return value;
    } catch (...) {
        throw ParseFailed("can't parse input into a number");
    }
}

Value process_string(const std::string& input) {
    if (input.size() >= 2 && input.front() == '(' && input.back() == ')') {
        return input.substr(1, input.size() - 2); // remove parentheses
    }
    throw ParseFailed("Not a string");
}

// Name constant parser
Value process_name_constant(const std::string& input) {
    if (!input.empty() && input[0] == '/') {
        return input;  // keep "/x" as string
    }
    throw ParseFailed("Not a name constant");
}

// Code block parser: "{ 1 add }" → ["1", "add"]
Value process_code_block(const std::string& input) {
    if (input.size() >= 2 && input.front() == '{' && input.back() == '}') {
        std::string inner = input.substr(1, input.size() - 2);

        std::istringstream iss(inner);
        std::vector<std::string> tokens;
        std::string token;

        while (iss >> token) {
            tokens.push_back(token);
        }

        return tokens;
    }

    throw ParseFailed("can't parse input into a code block");
}

using ParserFunc = Value(*)(const std::string&);

std::vector<ParserFunc> PARSERS = {
    process_boolean,
    process_number,
    process_string,
    process_name_constant,
    process_code_block
};

// ---------- MAIN DISPATCH ----------
Value process_constants(const std::string& input) {
    for (auto parser : PARSERS) {
        try {
            return parser(input);
        } catch (const ParseFailed&) {
            continue;
        }
    }

    throw ParseFailed("Could not parse: " + input);
}