#include "interpreter.h"
#include "parser.h"
#include <iostream>

void add_operation() {
    if (op_stack.size() < 2) {
        throw TypeMismatch("Not enough operands for add");
    }

    Value op1 = op_stack.back(); op_stack.pop_back();
    Value op2 = op_stack.back(); op_stack.pop_back();

    if (std::holds_alternative<int>(op1) && std::holds_alternative<int>(op2)) {
        int result = std::get<int>(op2) + std::get<int>(op1);
        op_stack.push_back(result);
    }
    else if (std::holds_alternative<double>(op1) && std::holds_alternative<double>(op2)) {
        double result = std::get<double>(op2) + std::get<double>(op1);
        op_stack.push_back(result);
    }
    else {
        throw TypeMismatch("Operands must be numbers");
    }
}

void mul_operation() {
    if (op_stack.size() < 2) {
        throw TypeMismatch("Not enough operands for mul");
    }

    Value op1 = op_stack.back(); op_stack.pop_back();
    Value op2 = op_stack.back(); op_stack.pop_back();

    // int * int
    if (std::holds_alternative<int>(op1) && std::holds_alternative<int>(op2)) {
        int result = std::get<int>(op2) * std::get<int>(op1);
        op_stack.push_back(result);
    }
    // double * double
    else if (std::holds_alternative<double>(op1) && std::holds_alternative<double>(op2)) {
        double result = std::get<double>(op2) * std::get<double>(op1);
        op_stack.push_back(result);
    }
    // mixed types (int + double)
    else if (
        (std::holds_alternative<int>(op1) && std::holds_alternative<double>(op2)) ||
        (std::holds_alternative<double>(op1) && std::holds_alternative<int>(op2))
    ) {
        double val1 = std::holds_alternative<int>(op1) ? std::get<int>(op1) : std::get<double>(op1);
        double val2 = std::holds_alternative<int>(op2) ? std::get<int>(op2) : std::get<double>(op2);

        double result = val2 * val1;
        op_stack.push_back(result);
    }
    else {
        throw TypeMismatch("Operands must be numbers");
    }
}

void pop_print_operation() {
    if (!op_stack.empty()) {
        Value v = op_stack.back();
        op_stack.pop_back();

        print_value(v);  
        std::cout << std::endl;
    }
    else {
        throw TypeMismatch("Stack is empty");
    }
}

void def_operation() {
    if (op_stack.size() < 2) {
        throw TypeMismatch("Not enough operands for def");
    }

    Value value = op_stack.back(); op_stack.pop_back();
    Value keyVal = op_stack.back(); op_stack.pop_back();

    if (!std::holds_alternative<std::string>(keyVal)) {
        throw TypeMismatch("Key must be a string");
    }

    std::string key = std::get<std::string>(keyVal);

    if (key.empty() || key[0] != '/') {
        throw TypeMismatch("Key must start with '/'");
    }

    key = key.substr(1);  // remove '/'

    dict_stack.back().dict[key] = value;
}

