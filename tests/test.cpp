#include <gtest/gtest.h>
#include "parser.h"
#include "interpreter.h"

extern void process_input(const std::string& input);
extern std::vector<Value> op_stack;
extern std::vector<PSDict*> dict_stack;
extern void init_interpreter();

// ---------- Helper to reset state ----------
void reset() {
    op_stack.clear();
    dict_stack.clear();
    init_interpreter();
}

// ---------- Boolean Parsing Tests ----------

TEST(BooleanParsing, ParseTrue) {
    reset();
    Value result = process_boolean("true");
    EXPECT_EQ(std::get<bool>(result), true);
}

TEST(BooleanParsing, ParseFalse) {
    reset();
    Value result = process_boolean("false");
    EXPECT_EQ(std::get<bool>(result), false);
}

TEST(BooleanParsing, ParseInvalid) {
    reset();
    EXPECT_THROW(process_boolean("something"), ParseFailed);
}

// ---------- Code Block Parsing Tests ----------

TEST(CodeBlockParsing, ParseCodeBlock) {
    reset();
    Value result = process_code_block("{ 1 add }");

    auto vec = std::get<std::vector<std::string>>(result);
    EXPECT_EQ(vec.size(), 2);
}

// ---------- Definition (def) Tests ----------

TEST(Operations, DefineAndLookup) {
    reset();

    process_input("/x");
    process_input("10");
    process_input("def");

    process_input("x");

    ASSERT_EQ(op_stack.size(), 1);
    EXPECT_EQ(std::get<int>(op_stack.back()), 10);
}

TEST(Operations, RedefineVariable) {
    reset();

    process_input("/x");
    process_input("5");
    process_input("def");

    process_input("/x");
    process_input("20");
    process_input("def");

    process_input("x");

    ASSERT_EQ(op_stack.size(), 1);
    EXPECT_EQ(std::get<int>(op_stack.back()), 20);
}