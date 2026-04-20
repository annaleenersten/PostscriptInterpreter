#include "parser.h"

#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <value.h>



/*
 * =========================
 * CUSTOM EXCEPTION TYPES
 * =========================
 *
 * ParseFailed:
 *  - Thrown when input cannot be interpreted as any valid literal/type
 *
 * TypeMismatch:
 *  - Thrown when a value type is semantically incorrect for an operation
 */

// Represents a general parsing failure with a descriptive message
ParseFailed::ParseFailed(const std::string& msg) : message(msg) {}

const char* ParseFailed::what() const noexcept {
    return message.c_str();
}

// Represents a runtime type error during interpretation
TypeMismatch::TypeMismatch(const std::string& msg) : message(msg) {}

const char* TypeMismatch::what() const noexcept {
    return message.c_str();
}


/* 
 * Helper function:
 * Converts a string to lowercase in-place.
 * Useful for case-insensitive parsing (if needed in extensions).
 */
std::string to_lower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

/*
 * =========================
 * PARSERS (LITERAL TYPES)
 * =========================
 *
 * Each function attempts to interpret the input as a specific type.
 * If parsing fails, it throws ParseFailed so the dispatcher can try next.
 */

/*
 * Parses boolean literals.
 * Accepts:
 *  - "true"  → true
 *  - "false" → false
 */
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

/*
 * Parses numeric literals.
 *
 * Behavior:
 *  - Uses std::stod for initial parsing
 *  - If value has no fractional part, returns int
 *  - Otherwise returns double
 */
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

/*
 * Parses string literals.
 *
 * Format:
 *  - Strings are enclosed in parentheses: (hello)
 *
 * Returns:
 *  - Content inside parentheses (without the delimiters)
 */
Value process_string(const std::string& input) {
    if (input.size() >= 2 && input.front() == '(' && input.back() == ')') {
        return input.substr(1, input.size() - 2); // remove parentheses
    }
    throw ParseFailed("Not a string");
}

/*
 * Parses name constants (symbols).
 *
 * Format:
 *  - Names start with '/'
 *  - Example: /x, /value
 *
 * Behavior:
 *  - Stored as raw string including '/'
 */
Value process_name_constant(const std::string& input) {
    if (!input.empty() && input[0] == '/') {
        return input;  // keep "/x" as string
    }
    throw ParseFailed("Not a name constant");
}

/*
 * Parses code blocks.
 *
 * Format:
 *  - { token1 token2 token3 }
 *
 * Behavior:
 *  - Removes outer braces
 *  - Splits inner content by whitespace
 *  - Produces vector of tokens (strings)
 *
 * Example:
 *  "{ 1 add }" → ["1", "add"]
 */
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

/*
 * =========================
 * PARSER REGISTRY
 * =========================
 *
 * List of all available parsers in priority order.
 * The interpreter tries each one sequentially until one succeeds.
 */
using ParserFunc = Value(*)(const std::string&);

std::vector<ParserFunc> PARSERS = {
    process_boolean,
    process_number,
    process_string,
    process_name_constant,
    process_code_block
};

/*
 * =========================
 * MAIN DISPATCH FUNCTION
 * =========================
 *
 * Attempts to parse input as any known literal type.
 *
 * Strategy:
 *  - Try each parser in PARSERS order
 *  - First successful parse is returned
 *  - If all fail → throw ParseFailed
 *
 * This implements a "first-match wins" parsing pipeline.
 */
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