#pragma once

#include "psdict.h"
#include "parser.h"
#include <vector>

extern std::vector<Value> op_stack;
extern std::vector<PSDict*> dict_stack;

void init_interpreter();
void lookup_in_dictionary(const std::string& input);
void process_input(const std::string& input);