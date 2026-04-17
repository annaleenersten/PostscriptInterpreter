#include <gtest/gtest.h>
#include "parser.h"
#include "interpreter.h"

extern void process_input(const std::string& input);
extern std::vector<Value> op_stack;
extern std::vector<PSDict*> dict_stack;
extern void init_interpreter();

extern void reset();


// ================================================
//  Stack Manipulation Tests
// ================================================

TEST(StackOps, ExchBasic) {
    reset();

    process_input("1");
    process_input("2");
    process_input("exch");

    ASSERT_EQ(op_stack.size(), 2);
    EXPECT_EQ(std::get<int>(op_stack[0]), 2);
    EXPECT_EQ(std::get<int>(op_stack[1]), 1);
}

TEST(StackOps, ExchNotEnoughOperands) {
    reset();

    process_input("1");
    EXPECT_THROW(process_input("exch"), TypeMismatch);
}

TEST(StackOps, PopRemovesTop) {
    reset();

    process_input("1");
    process_input("2");
    process_input("pop");

    ASSERT_EQ(op_stack.size(), 1);
    EXPECT_EQ(std::get<int>(op_stack[0]), 1);
}

TEST(StackOps, DupDuplicatesTop) {
    reset();

    process_input("5");
    process_input("dup");

    ASSERT_EQ(op_stack.size(), 2);
    EXPECT_EQ(std::get<int>(op_stack[0]), 5);
    EXPECT_EQ(std::get<int>(op_stack[1]), 5);
}

TEST(StackOps, CopyTopN) {
    reset();

    process_input("1");
    process_input("2");
    process_input("3");
    process_input("2");   
    process_input("copy");

    ASSERT_EQ(op_stack.size(), 5);

    EXPECT_EQ(std::get<int>(op_stack[0]), 1);
    EXPECT_EQ(std::get<int>(op_stack[1]), 2);
    EXPECT_EQ(std::get<int>(op_stack[2]), 3);
    EXPECT_EQ(std::get<int>(op_stack[3]), 2);
    EXPECT_EQ(std::get<int>(op_stack[4]), 3);
}

TEST(StackOps, CopyInvalidN) {
    reset();

    process_input("1");
    process_input("2");
    process_input("5"); 

    EXPECT_THROW(process_input("copy"), TypeMismatch);
}

TEST(StackOps, ClearEmptiesStack) {
    reset();

    process_input("1");
    process_input("2");
    process_input("clear");

    EXPECT_TRUE(op_stack.empty());
}

TEST(StackOps, CountPushesSize) {
    reset();

    process_input("1");
    process_input("2");
    process_input("count");

    ASSERT_EQ(op_stack.size(), 3);
    EXPECT_EQ(std::get<int>(op_stack.back()), 2);
}