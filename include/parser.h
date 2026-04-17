#pragma once

#include <string>
#include <vector>
#include <exception>
#include "value.h"

class ParseFailed : public std::exception {
public:
    explicit ParseFailed(const std::string& msg);
    const char* what() const noexcept override;
private:
    std::string message;
};

class TypeMismatch : public std::exception {
public:
    explicit TypeMismatch(const std::string& msg);
    const char* what() const noexcept override;
private:
    std::string message;
};

using ParserFunc = Value(*)(const std::string&);

Value process_boolean(const std::string&);
Value process_number(const std::string&);
Value process_name_constant(const std::string&);
Value process_code_block(const std::string&);
Value process_constants(const std::string&);

extern std::vector<ParserFunc> PARSERS;