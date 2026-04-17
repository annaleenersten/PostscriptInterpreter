#include <gtest/gtest.h>
#include "parser.h"
#include "interpreter.h"

extern void process_input(const std::string& input);
extern std::vector<Value> op_stack;
extern std::vector<PSDict*> dict_stack;
extern void init_interpreter();

extern void reset();

// ================================================
//  Boolean and Bitwise Operations Tests
// ================================================

TEST(ComparisonOps, EqNumbers) {
    reset();

    process_input("5");
    process_input("5");
    process_input("eq");

    EXPECT_EQ(std::get<bool>(op_stack.back()), true);
}

TEST(ComparisonOps, EqStrings) {
    reset();

    process_input("(hi)");
    process_input("(hi)");
    process_input("eq");

    EXPECT_EQ(std::get<bool>(op_stack.back()), true);
}

TEST(ComparisonOps, EqMixedNumbers) {
    reset();

    process_input("5");
    process_input("5.0");
    process_input("eq");

    EXPECT_EQ(std::get<bool>(op_stack.back()), true);
}

TEST(ComparisonOps, NeBasic) {
    reset();

    process_input("5");
    process_input("6");
    process_input("ne");

    EXPECT_EQ(std::get<bool>(op_stack.back()), true);
}

TEST(ComparisonOps, GtNumbers) {
    reset();

    process_input("10");
    process_input("5");
    process_input("gt");

    EXPECT_EQ(std::get<bool>(op_stack.back()), true);
}

TEST(ComparisonOps, LtNumbers) {
    reset();

    process_input("3");
    process_input("8");
    process_input("lt");

    EXPECT_EQ(std::get<bool>(op_stack.back()), true);
}

TEST(ComparisonOps, GeEqual) {
    reset();

    process_input("5");
    process_input("5");
    process_input("ge");

    EXPECT_EQ(std::get<bool>(op_stack.back()), true);
}

TEST(ComparisonOps, LeEqual) {
    reset();

    process_input("5");
    process_input("5");
    process_input("le");

    EXPECT_EQ(std::get<bool>(op_stack.back()), true);
}

TEST(ComparisonOps, StringComparison) {
    reset();

    process_input("(apple)");
    process_input("(banana)");
    process_input("lt");

    EXPECT_EQ(std::get<bool>(op_stack.back()), true);
}

TEST(ComparisonOps, TypeMismatchNumberString) {
    reset();

    process_input("5");
    process_input("(hello)");

    EXPECT_THROW(process_input("eq"), TypeMismatch);
}

TEST(ComparisonOps, TypeMismatchBoolNumber) {
    reset();

    process_input("true");
    process_input("5");

    EXPECT_THROW(process_input("gt"), TypeMismatch);
}

TEST(ComparisonOps, StackUnderflow) {
    reset();

    process_input("5");

    EXPECT_THROW(process_input("eq"), std::runtime_error);
}

TEST(BooleanOps, TruePushesValue) {
    reset();

    process_input("true");

    ASSERT_FALSE(op_stack.empty());
    EXPECT_EQ(std::get<bool>(op_stack.back()), true);
}

TEST(BooleanOps, FalsePushesValue) {
    reset();

    process_input("false");

    ASSERT_FALSE(op_stack.empty());
    EXPECT_EQ(std::get<bool>(op_stack.back()), false);
}

TEST(BooleanOps, AndBooleans) {
    reset();

    process_input("true");
    process_input("false");
    process_input("and");

    EXPECT_EQ(std::get<bool>(op_stack.back()), false);
}

TEST(BooleanOps, AndBooleansTrue) {
    reset();

    process_input("true");
    process_input("true");
    process_input("and");

    EXPECT_EQ(std::get<bool>(op_stack.back()), true);
}

TEST(BooleanOps, AndIntegersBitwise) {
    reset();

    process_input("6");   // 110
    process_input("3");   // 011
    process_input("and");

    EXPECT_EQ(std::get<int>(op_stack.back()), 2); // 010
}

TEST(BooleanOps, OrBooleans) {
    reset();

    process_input("true");
    process_input("false");
    process_input("or");

    EXPECT_EQ(std::get<bool>(op_stack.back()), true);
}

TEST(BooleanOps, OrIntegersBitwise) {
    reset();

    process_input("6");   // 110
    process_input("3");   // 011
    process_input("or");

    EXPECT_EQ(std::get<int>(op_stack.back()), 7); // 111
}

TEST(BooleanOps, NotBoolean) {
    reset();

    process_input("true");
    process_input("not");

    EXPECT_EQ(std::get<bool>(op_stack.back()), false);
}

TEST(BooleanOps, NotIntegerBitwise) {
    reset();

    process_input("5");   // 0101
    process_input("not");

    EXPECT_EQ(std::get<int>(op_stack.back()), ~5);
}