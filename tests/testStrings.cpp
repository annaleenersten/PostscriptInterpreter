#include <gtest/gtest.h>
#include "parser.h"
#include "interpreter.h"

extern void process_input(const std::string& input);
extern std::vector<Value> op_stack;
extern std::vector<PSDict*> dict_stack;
extern void reset();

// ================================================
//  String Operation Tests
// ================================================

TEST(StringOps, StringLengthBasic) {
    reset();

    process_input("(hello)");
    process_input("strlen");

    ASSERT_FALSE(op_stack.empty());
    EXPECT_EQ(std::get<int>(op_stack.back()), 5);
}

TEST(StringOps, StringLengthEmpty) {
    reset();

    process_input("()");
    process_input("strlen");

    EXPECT_EQ(std::get<int>(op_stack.back()), 0);
}

TEST(StringOps, StringLengthTypeError) {
    reset();

    process_input("10");
    EXPECT_THROW(process_input("strlen"), TypeMismatch);
}

TEST(StringOps, GetCharacterBasic) {
    reset();

    process_input("(hello)");
    process_input("1");
    process_input("get");

    EXPECT_EQ(std::get<int>(op_stack.back()), (int)'e');
}

TEST(StringOps, GetCharacterOutOfBounds) {
    reset();

    process_input("(hi)");
    process_input("5");

    EXPECT_THROW(process_input("get"), std::out_of_range);
}

TEST(StringOps, GetCharacterTypeError) {
    reset();

    process_input("(hello)");
    process_input("(oops)");

    EXPECT_THROW(process_input("get"), TypeMismatch);
}

TEST(StringOps, GetIntervalBasic) {
    reset();

    process_input("(hello)");
    process_input("1");
    process_input("3");
    process_input("getinterval");

    EXPECT_EQ(std::get<std::string>(op_stack.back()), "ell");
}

TEST(StringOps, GetIntervalFullString) {
    reset();

    process_input("(hello)");
    process_input("0");
    process_input("5");
    process_input("getinterval");

    EXPECT_EQ(std::get<std::string>(op_stack.back()), "hello");
}

TEST(StringOps, GetIntervalOutOfBounds) {
    reset();

    process_input("(hello)");
    process_input("3");
    process_input("10");

    EXPECT_THROW(process_input("getinterval"), std::out_of_range);
}

TEST(StringOps, PutIntervalBasic) {
    reset();

    process_input("(hello)");
    process_input("1");
    process_input("(XYZ)");
    process_input("putinterval");

    EXPECT_EQ(std::get<std::string>(op_stack.back()), "hXYZo");
}

TEST(StringOps, PutIntervalReplaceBeginning) {
    reset();

    process_input("(hello)");
    process_input("0");
    process_input("(A)");
    process_input("putinterval");

    EXPECT_EQ(std::get<std::string>(op_stack.back()), "Aello");
}

TEST(StringOps, PutIntervalOutOfBounds) {
    reset();

    process_input("(hello)");
    process_input("4");
    process_input("(XYZ)");

    EXPECT_THROW(process_input("putinterval"), std::out_of_range);
}

TEST(StringOps, PutIntervalTypeError) {
    reset();

    process_input("(hello)");
    process_input("(oops)");
    process_input("(bad)");

    EXPECT_THROW(process_input("putinterval"), TypeMismatch);
}

