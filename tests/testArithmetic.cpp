#include <gtest/gtest.h>
#include "parser.h"
#include "interpreter.h"

extern void process_input(const std::string& input);
extern std::vector<Value> op_stack;
extern std::vector<PSDict*> dict_stack;
extern void init_interpreter();

extern void reset();

// ================================================
//  Arithmetic Operation Tests
// ================================================

TEST(Arithmetic, AddIntegers) {
    op_stack.push_back(2);
    op_stack.push_back(3);
    process_input("add");

    EXPECT_EQ(std::get<int>(op_stack.back()), 5);
}

TEST(Arithmetic, SubIntegers) {
    op_stack.push_back(5);
    op_stack.push_back(3);
    process_input("sub");

    EXPECT_EQ(std::get<int>(op_stack.back()), 2);
}

TEST(Arithmetic, MulIntegers) {
    op_stack.push_back(4);
    op_stack.push_back(3);
    process_input("mul");

    EXPECT_EQ(std::get<int>(op_stack.back()), 12);
}

TEST(Arithmetic, DivDoubles) {
    op_stack.push_back(10.0);
    op_stack.push_back(2.0);
    process_input("div");

    EXPECT_EQ(std::get<double>(op_stack.back()), 5.0);
}

TEST(Arithmetic, IntegerDivision) {
    op_stack.push_back(7);
    op_stack.push_back(2);
    process_input("idiv");

    EXPECT_EQ(std::get<int>(op_stack.back()), 3);
}

TEST(Arithmetic, Modulus) {
    op_stack.push_back(7);
    op_stack.push_back(3);
    process_input("mod");

    EXPECT_EQ(std::get<int>(op_stack.back()), 1);
}

TEST(Arithmetic, AbsoluteValue) {
    op_stack.push_back(-5);
    process_input("abs");

    EXPECT_EQ(std::get<int>(op_stack.back()), 5);
}

TEST(Arithmetic, Negation) {
    op_stack.push_back(5);
    process_input("neg");

    EXPECT_EQ(std::get<int>(op_stack.back()), -5);
}

TEST(Arithmetic, Ceiling) {
    op_stack.push_back(3.2);
    process_input("ceiling");

    EXPECT_EQ(std::get<double>(op_stack.back()), 4.0);
}

TEST(Arithmetic, FloorPositive) {
    op_stack.push_back(3.8);
    process_input("floor");

    EXPECT_EQ(std::get<double>(op_stack.back()), 3.0);
}

TEST(Arithmetic, FloorNegative) {
    op_stack.push_back(-4.8);
    process_input("floor");

    EXPECT_EQ(std::get<double>(op_stack.back()), -5.0);
}

TEST(Arithmetic, RoundValue) {
    op_stack.push_back(3.6);
    process_input("round");

    EXPECT_EQ(std::get<double>(op_stack.back()), 4.0);
}

TEST(Arithmetic, SquareRoot) {
    op_stack.push_back(9.0);
    process_input("sqrt");

    EXPECT_EQ(std::get<double>(op_stack.back()), 3.0);
}