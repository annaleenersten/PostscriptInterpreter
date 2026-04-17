#include <gtest/gtest.h>
#include "parser.h"
#include "interpreter.h"

extern void process_input(const std::string& input);
extern std::vector<Value> op_stack;
extern std::vector<PSDict*> dict_stack;
extern void init_interpreter();


// ================================================
//  Parser Tests
// ================================================


// Helper to reset state
void reset() {
    op_stack.clear();
    dict_stack.clear();
    init_interpreter();
}


// ================================================
//  Booleans
// ================================================

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


// ================================================
//  Numbers
// ================================================

TEST(NumberParsing, ParseInteger) {
    Value v = process_number("42");
    EXPECT_EQ(std::get<int>(v), 42);
}

TEST(NumberParsing, ParseDouble) {
    Value v = process_number("3.14");
    EXPECT_DOUBLE_EQ(std::get<double>(v), 3.14);
}

TEST(NumberParsing, ParseInvalidNumber) {
    EXPECT_THROW(process_number("abc"), ParseFailed);
}


// ================================================
//  Name Constants
// ================================================

TEST(NameConstantParsing, ParseValidNameConstant) {
    Value v = process_name_constant("/x");
    EXPECT_EQ(std::get<std::string>(v), "/x");
}

TEST(NameConstantParsing, ParseInvalidNameConstant) {
    EXPECT_THROW(process_name_constant("x"), ParseFailed);
}


// ================================================
//  Strings
// ================================================

TEST(StringParsing, ParseSimpleString) {
    Value v = process_string("(hello)");
    EXPECT_EQ(std::get<std::string>(v), "hello");
}

TEST(StringParsing, ParseEmptyString) {
    Value v = process_string("()");
    EXPECT_EQ(std::get<std::string>(v), "");
}

TEST(StringParsing, ParseInvalidStringThrows) {
    EXPECT_THROW(process_string("hello"), ParseFailed);
    EXPECT_THROW(process_string("(hello"), ParseFailed);
    EXPECT_THROW(process_string("hello)"), ParseFailed);
}


// ================================================
//  Code Blocks
// ================================================

TEST(CodeBlockParsing, ParseCodeBlockMultipleTokens) {
    Value v = process_code_block("{ 1 add 2 sub }");

    auto vec = std::get<std::vector<std::string>>(v);
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[0], "1");
    EXPECT_EQ(vec[1], "add");
}

TEST(CodeBlockParsing, ParseCodeBlock) {
    reset();
    Value result = process_code_block("{ 1 add }");

    auto vec = std::get<std::vector<std::string>>(result);
    EXPECT_EQ(vec.size(), 2);
}


// ================================================
//  Process Constants 
// ================================================

TEST(ParserDispatch, ProcessConstantsBoolean) {
    Value v = process_constants("true");
    EXPECT_EQ(std::get<bool>(v), true);
}

TEST(ParserDispatch, ProcessConstantsFalse) {
    Value v = process_constants("false");
    EXPECT_EQ(std::get<bool>(v), false);
}

TEST(ParserDispatch, ProcessConstantsNumber) {
    Value v = process_constants("123");
    EXPECT_EQ(std::get<int>(v), 123);
}

TEST(ParserDispatch, ProcessConstantsDouble) {
    Value v = process_constants("2.5");
    EXPECT_DOUBLE_EQ(std::get<double>(v), 2.5);
}

TEST(ParserDispatch, ProcessConstantsString) {
    Value v = process_constants("(hello)");
    EXPECT_EQ(std::get<std::string>(v), "hello");
}

TEST(ParserDispatch, ProcessConstantsName) {
    Value v = process_constants("/hello");
    EXPECT_EQ(std::get<std::string>(v), "/hello");
}

TEST(ParserDispatch, ProcessConstantsCodeBlock) {
    Value v = process_constants("{ 1 add }");

    auto vec = std::get<std::vector<std::string>>(v);
    EXPECT_EQ(vec.size(), 2);
}

TEST(ParserDispatch, ProcessConstantsInvalidThrows) {
    EXPECT_THROW(process_constants("???"), ParseFailed);
}

