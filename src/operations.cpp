#include "interpreter.h"
#include "parser.h"
#include <iostream>
#include <cmath>

void exch_operation() {
    if (op_stack.size() < 2) {
        throw TypeMismatch("Not enough operands for exch");
    }

    Value op1 = op_stack.back(); op_stack.pop_back();
    Value op2 = op_stack.back(); op_stack.pop_back();

    op_stack.push_back(op1);
    op_stack.push_back(op2);
}

void pop_operation() {
    if (op_stack.empty()) {
        throw TypeMismatch("Stack is empty");
    }

    op_stack.pop_back();
}

void copy_operation() {
    if (op_stack.empty()) {
        throw TypeMismatch("Stack is empty");
    }

    Value nVal = op_stack.back(); op_stack.pop_back();

    if (!std::holds_alternative<int>(nVal)) {
        throw TypeMismatch("copy expects an integer");
    }

    int n = std::get<int>(nVal);

    if (n < 0 || op_stack.size() < static_cast<size_t>(n)) {
        throw TypeMismatch("Not enough elements to copy");
    }

    std::vector<Value> temp(op_stack.end() - n, op_stack.end());

    for (const auto& v : temp) {
        op_stack.push_back(v);
    }
}

void dup_operation() {
    if (op_stack.empty()) {
        throw TypeMismatch("Stack is empty");
    }

    op_stack.push_back(op_stack.back());
}

void clear_operation() {
    op_stack.clear();
}

void count_operation() {
    op_stack.push_back(static_cast<int>(op_stack.size()));
}





double to_double(const Value& v) {
    if (std::holds_alternative<int>(v)) return std::get<int>(v);
    if (std::holds_alternative<double>(v)) return std::get<double>(v);
    throw TypeMismatch("Operand must be a number");
}

void add_operation() {
    if (op_stack.size() < 2)
        throw TypeMismatch("Not enough operands for add");

    Value op1 = op_stack.back(); op_stack.pop_back();
    Value op2 = op_stack.back(); op_stack.pop_back();

    double result = to_double(op2) + to_double(op1);

    if (std::floor(result) == result)
        op_stack.push_back((int)result);
    else
        op_stack.push_back(result);
}

void sub_operation() {
    if (op_stack.size() < 2)
        throw TypeMismatch("Not enough operands for sub");

    Value op1 = op_stack.back(); op_stack.pop_back();
    Value op2 = op_stack.back(); op_stack.pop_back();

    double result = to_double(op2) - to_double(op1);

    if (std::floor(result) == result)
        op_stack.push_back((int)result);
    else
        op_stack.push_back(result);
}

void mul_operation() {
    if (op_stack.size() < 2)
        throw TypeMismatch("Not enough operands for mul");

    Value op1 = op_stack.back(); op_stack.pop_back();
    Value op2 = op_stack.back(); op_stack.pop_back();

    double result = to_double(op2) * to_double(op1);

    if (std::floor(result) == result)
        op_stack.push_back((int)result);
    else
        op_stack.push_back(result);
}

void div_operation() {
    if (op_stack.size() < 2)
        throw TypeMismatch("Not enough operands for div");

    Value op1 = op_stack.back(); op_stack.pop_back();
    Value op2 = op_stack.back(); op_stack.pop_back();

    double divisor = to_double(op1);
    if (divisor == 0)
        throw TypeMismatch("Division by zero");

    double result = to_double(op2) / divisor;
    op_stack.push_back(result);
}

void idiv_operation() {
    if (op_stack.size() < 2)
        throw TypeMismatch("Not enough operands for idiv");

    Value op1 = op_stack.back(); op_stack.pop_back();
    Value op2 = op_stack.back(); op_stack.pop_back();

    if (!std::holds_alternative<int>(op1) || !std::holds_alternative<int>(op2))
        throw TypeMismatch("idiv requires integers");

    int divisor = std::get<int>(op1);
    if (divisor == 0)
        throw TypeMismatch("Division by zero");

    int result = std::get<int>(op2) / divisor;
    op_stack.push_back(result);
}

void mod_operation() {
    if (op_stack.size() < 2)
        throw TypeMismatch("Not enough operands for mod");

    Value op1 = op_stack.back(); op_stack.pop_back();
    Value op2 = op_stack.back(); op_stack.pop_back();

    if (!std::holds_alternative<int>(op1) || !std::holds_alternative<int>(op2))
        throw TypeMismatch("mod requires integers");

    int divisor = std::get<int>(op1);
    if (divisor == 0)
        throw TypeMismatch("Modulo by zero");

    int result = std::get<int>(op2) % divisor;
    op_stack.push_back(result);
}

void abs_operation() {
    if (op_stack.empty())
        throw TypeMismatch("Not enough operands for abs");

    Value v = op_stack.back(); op_stack.pop_back();

    if (std::holds_alternative<int>(v)) {
        op_stack.push_back(std::abs(std::get<int>(v)));
    }
    else if (std::holds_alternative<double>(v)) {
        op_stack.push_back(std::abs(std::get<double>(v)));
    }
    else {
        throw TypeMismatch("abs requires a number");
    }
}

void neg_operation() {
    if (op_stack.empty())
        throw TypeMismatch("Not enough operands for neg");

    Value v = op_stack.back(); op_stack.pop_back();

    if (std::holds_alternative<int>(v)) {
        op_stack.push_back(-std::get<int>(v));
    }
    else if (std::holds_alternative<double>(v)) {
        op_stack.push_back(-std::get<double>(v));
    }
    else {
        throw TypeMismatch("neg requires a number");
    }
}

void ceiling_operation() {
    if (op_stack.empty())
        throw TypeMismatch("Not enough operands for ceiling");

    double val = to_double(op_stack.back());
    op_stack.pop_back();

    op_stack.push_back((double)std::ceil(val));
}

void floor_operation() {
    if (op_stack.empty())
        throw TypeMismatch("Not enough operands for floor");

    double val = to_double(op_stack.back());
    op_stack.pop_back();

    op_stack.push_back((double)std::floor(val));
}

void round_operation() {
    if (op_stack.empty())
        throw TypeMismatch("Not enough operands for round");

    double val = to_double(op_stack.back());
    op_stack.pop_back();

    op_stack.push_back((double)std::round(val));
}

void sqrt_operation() {
    if (op_stack.empty())
        throw TypeMismatch("Not enough operands for sqrt");

    double val = to_double(op_stack.back());
    op_stack.pop_back();

    if (val < 0)
        throw TypeMismatch("sqrt of negative number");

    op_stack.push_back(std::sqrt(val));
}








void dict_operation() {
    Value v = op_stack.back();
    op_stack.pop_back();

    int size = std::get<int>(v);

    PSDict* d = new PSDict(size);
    d->dict.reserve(size);

    op_stack.push_back(d);   
}

void length_operation() {
    if (dict_stack.empty()) {
        throw std::runtime_error("No active dictionary");
    }

    PSDict* d = dict_stack.back();
    op_stack.push_back((int)d->dict.size());
}

void maxlength_operation() {
    Value v = op_stack.back();
    op_stack.pop_back();

    if (!std::holds_alternative<PSDict*>(v)) {
        throw std::runtime_error("maxlength expects a dict");
    }

    PSDict* d = std::get<PSDict*>(v);
    op_stack.push_back(d->capacity);
}

void begin_operation() {
    Value v = op_stack.back();
    op_stack.pop_back();

    if (!std::holds_alternative<PSDict*>(v)) {
        throw std::runtime_error("begin expects dict");
    }

    dict_stack.push_back(std::get<PSDict*>(v));
    std::cout << "BEGIN dict_stack size = " << dict_stack.size() << "\n";
}

void end_operation() {
    if (dict_stack.empty()) {
        throw std::runtime_error("dict stack underflow");
    }

    dict_stack.pop_back();
    std::cout << "END dict_stack size = " << dict_stack.size() << "\n";
}

void def_operation() {
    if (op_stack.size() < 2) {
        throw TypeMismatch("Not enough operands for def");
    }

    Value value = op_stack.back(); op_stack.pop_back();
    Value keyVal = op_stack.back(); op_stack.pop_back();

    if (!std::holds_alternative<std::string>(keyVal)) {
        throw TypeMismatch("Key must be string");
    }

    std::string key = std::get<std::string>(keyVal);

    if (key.empty() || key[0] != '/') {
        throw TypeMismatch("Key must start with '/'");
    }

    key = key.substr(1);

    dict_stack.back()->dict[key] = value;
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
