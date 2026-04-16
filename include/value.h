#pragma once
#include <variant>
#include <string>
#include <vector>
#include <functional>
#include <iostream>

class PSDict; 

using Value = std::variant<
    bool,
    int,
    double,
    std::string,
    std::vector<std::string>,
    std::function<void()>,
    PSDict*
>;

void print_value(const Value& v); 

