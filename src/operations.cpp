#include "interpreter.h"
#include "parser.h"
#include <iostream>
#include <cmath>


// ================================================
//  Stack Manipulation Operations
// ================================================

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


// ================================================
//  Arithmetic operations
// ================================================

double to_double(const Value& v) {
    if (std::holds_alternative<int>(v)){
        return std::get<int>(v);
    }
    if (std::holds_alternative<double>(v)){
        return std::get<double>(v);
    }
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


// ================================================
//  Dictionary operations
// ================================================

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


// ================================================
//  String operations
// ================================================

void string_length_operation() {
    if (op_stack.empty()) {
        throw std::runtime_error("stack underflow");
    }

    Value v = op_stack.back();
    op_stack.pop_back();

    if (!std::holds_alternative<std::string>(v)) {
        throw TypeMismatch("length expects a string");
    }

    const std::string& s = std::get<std::string>(v);
    op_stack.push_back((int)s.size());
}

void get_string_operation() {
    if (op_stack.size() < 2) {
        throw std::runtime_error("stack underflow");
    }

    Value indexVal = op_stack.back(); op_stack.pop_back();
    Value strVal   = op_stack.back(); op_stack.pop_back();

    if (!std::holds_alternative<int>(indexVal) ||
        !std::holds_alternative<std::string>(strVal)) {
        throw TypeMismatch("get expects (string, int)");
    }

    int index = std::get<int>(indexVal);
    const std::string& s = std::get<std::string>(strVal);

    if (index < 0 || index >= (int)s.size()) {
        throw std::out_of_range("index out of bounds");
    }

    op_stack.push_back((int)s[index]);
}

void getinterval_operation() {
    if (op_stack.size() < 3) {
        throw std::runtime_error("stack underflow");
    }

    Value countVal = op_stack.back(); op_stack.pop_back();
    Value indexVal = op_stack.back(); op_stack.pop_back();
    Value strVal   = op_stack.back(); op_stack.pop_back();

    if (!std::holds_alternative<int>(countVal) ||
        !std::holds_alternative<int>(indexVal) ||
        !std::holds_alternative<std::string>(strVal)) {
        throw TypeMismatch("getinterval expects (string, int, int)");
    }

    int count = std::get<int>(countVal);
    int index = std::get<int>(indexVal);
    const std::string& s = std::get<std::string>(strVal);

    if (index < 0 || count < 0 || index + count > (int)s.size()) {
        throw std::out_of_range("invalid substring range");
    }

    op_stack.push_back(s.substr(index, count));
}

void putinterval_operation() {
    if (op_stack.size() < 3) {
        throw std::runtime_error("stack underflow");
    }

    Value srcVal    = op_stack.back(); op_stack.pop_back();
    Value indexVal  = op_stack.back(); op_stack.pop_back();
    Value targetVal = op_stack.back(); op_stack.pop_back();

    if (!std::holds_alternative<std::string>(srcVal) ||
        !std::holds_alternative<int>(indexVal) ||
        !std::holds_alternative<std::string>(targetVal)) {
        throw TypeMismatch("putinterval expects (string, int, string)");
    }

    std::string src    = std::get<std::string>(srcVal);
    int index          = std::get<int>(indexVal);
    std::string target = std::get<std::string>(targetVal);

    if (index < 0 || index + (int)src.size() > (int)target.size()) {
        throw std::out_of_range("putinterval out of bounds");
    }

    target.replace(index, src.size(), src);

    op_stack.push_back(target);
}

// ================================================
//  Boolean and Bitwise operations
// ================================================

void eq_operation() {
    if (op_stack.size() < 2) throw std::runtime_error("stack underflow");

    Value b = op_stack.back(); op_stack.pop_back();
    Value a = op_stack.back(); op_stack.pop_back();

    // number comparison
    if ((std::holds_alternative<int>(a) || std::holds_alternative<double>(a)) &&
        (std::holds_alternative<int>(b) || std::holds_alternative<double>(b))) {
        op_stack.push_back(to_double(a) == to_double(b));
        return;
    }

    // string comparison
    if (std::holds_alternative<std::string>(a) &&
        std::holds_alternative<std::string>(b)) {
        op_stack.push_back(std::get<std::string>(a) == std::get<std::string>(b));
        return;
    }

    // bool comparison
    if (std::holds_alternative<bool>(a) &&
        std::holds_alternative<bool>(b)) {
        op_stack.push_back(std::get<bool>(a) == std::get<bool>(b));
        return;
    }

    throw TypeMismatch("eq type mismatch");
}

void ne_operation() {
    eq_operation();
    Value v = op_stack.back(); op_stack.pop_back();
    op_stack.push_back(!std::get<bool>(v));
}

void gt_operation() {
    if (op_stack.size() < 2) throw std::runtime_error("stack underflow");

    Value b = op_stack.back(); op_stack.pop_back();
    Value a = op_stack.back(); op_stack.pop_back();

    // numbers
    if ((std::holds_alternative<int>(a) || std::holds_alternative<double>(a)) &&
        (std::holds_alternative<int>(b) || std::holds_alternative<double>(b))) {
        op_stack.push_back(to_double(a) > to_double(b));
        return;
    }

    // strings (lexicographic)
    if (std::holds_alternative<std::string>(a) &&
        std::holds_alternative<std::string>(b)) {
        op_stack.push_back(std::get<std::string>(a) > std::get<std::string>(b));
        return;
    }

    throw TypeMismatch("gt type mismatch");
}

void ge_operation() {
    if (op_stack.size() < 2) throw std::runtime_error("stack underflow");

    Value b = op_stack.back(); op_stack.pop_back();
    Value a = op_stack.back(); op_stack.pop_back();

    if ((std::holds_alternative<int>(a) || std::holds_alternative<double>(a)) &&
        (std::holds_alternative<int>(b) || std::holds_alternative<double>(b))) {
        op_stack.push_back(to_double(a) >= to_double(b));
        return;
    }

    if (std::holds_alternative<std::string>(a) &&
        std::holds_alternative<std::string>(b)) {
        op_stack.push_back(std::get<std::string>(a) >= std::get<std::string>(b));
        return;
    }

    throw TypeMismatch("ge type mismatch");
}

void lt_operation() {
    if (op_stack.size() < 2) throw std::runtime_error("stack underflow");

    Value b = op_stack.back(); op_stack.pop_back();
    Value a = op_stack.back(); op_stack.pop_back();

    if ((std::holds_alternative<int>(a) || std::holds_alternative<double>(a)) &&
        (std::holds_alternative<int>(b) || std::holds_alternative<double>(b))) {
        op_stack.push_back(to_double(a) < to_double(b));
        return;
    }

    if (std::holds_alternative<std::string>(a) &&
        std::holds_alternative<std::string>(b)) {
        op_stack.push_back(std::get<std::string>(a) < std::get<std::string>(b));
        return;
    }

    throw TypeMismatch("lt type mismatch");
}

void le_operation() {
    if (op_stack.size() < 2) throw std::runtime_error("stack underflow");

    Value b = op_stack.back(); op_stack.pop_back();
    Value a = op_stack.back(); op_stack.pop_back();

    if ((std::holds_alternative<int>(a) || std::holds_alternative<double>(a)) &&
        (std::holds_alternative<int>(b) || std::holds_alternative<double>(b))) {
        op_stack.push_back(to_double(a) <= to_double(b));
        return;
    }

    if (std::holds_alternative<std::string>(a) &&
        std::holds_alternative<std::string>(b)) {
        op_stack.push_back(std::get<std::string>(a) <= std::get<std::string>(b));
        return;
    }

    throw TypeMismatch("le type mismatch");
}

void and_operation() {
    if (op_stack.size() < 2) {
        throw std::runtime_error("stack underflow");
    }

    Value b = op_stack.back(); op_stack.pop_back();
    Value a = op_stack.back(); op_stack.pop_back();

    // bool AND
    if (std::holds_alternative<bool>(a) &&
        std::holds_alternative<bool>(b)) {
        op_stack.push_back(std::get<bool>(a) && std::get<bool>(b));
        return;
    }

    // int AND (bitwise)
    if (std::holds_alternative<int>(a) &&
        std::holds_alternative<int>(b)) {
        op_stack.push_back(std::get<int>(a) & std::get<int>(b));
        return;
    }

    throw TypeMismatch("and type mismatch");
}

void or_operation() {
    if (op_stack.size() < 2) {
        throw std::runtime_error("stack underflow");
    }

    Value b = op_stack.back(); op_stack.pop_back();
    Value a = op_stack.back(); op_stack.pop_back();

    // bool OR
    if (std::holds_alternative<bool>(a) &&
        std::holds_alternative<bool>(b)) {
        op_stack.push_back(std::get<bool>(a) || std::get<bool>(b));
        return;
    }

    // int OR (bitwise)
    if (std::holds_alternative<int>(a) &&
        std::holds_alternative<int>(b)) {
        op_stack.push_back(std::get<int>(a) | std::get<int>(b));
        return;
    }

    throw TypeMismatch("or type mismatch");
}

void not_operation() {
    if (op_stack.empty()) {
        throw std::runtime_error("stack underflow");
    }

    Value v = op_stack.back();
    op_stack.pop_back();

    // bool NOT
    if (std::holds_alternative<bool>(v)) {
        op_stack.push_back(!std::get<bool>(v));
        return;
    }

    // int NOT (bitwise)
    if (std::holds_alternative<int>(v)) {
        op_stack.push_back(~std::get<int>(v));
        return;
    }

    throw TypeMismatch("not type mismatch");
}

void true_operation() {
    op_stack.push_back(true);
}

void false_operation() {
    op_stack.push_back(false);
}





// ================================================
//  INPUT/OUTPUT operations
// ================================================

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
