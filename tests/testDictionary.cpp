#include <gtest/gtest.h>
#include "parser.h"
#include "interpreter.h"

extern void process_input(const std::string& input);
extern std::vector<Value> op_stack;
extern std::vector<PSDict*> dict_stack;
extern void reset();

// ---------- Dictionary Tests ----------

TEST(DictionaryOps, DictCreatesEmptyDictionary) {
    reset();

    process_input("10");
    process_input("dict");

    ASSERT_FALSE(op_stack.empty());
    ASSERT_TRUE(std::holds_alternative<PSDict*>(op_stack.back()));

    PSDict* d = std::get<PSDict*>(op_stack.back());

    EXPECT_EQ(d->dict.size(), 0);
    EXPECT_EQ(d->capacity, 10);
}

TEST(DictionaryOps, MaxLengthReturnsValue) {
    reset();

    process_input("10");
    process_input("dict");
    process_input("maxlength");

    int cap = std::get<int>(op_stack.back());
    EXPECT_EQ(cap, 10);
}

TEST(DictionaryOps, LengthReturnsCorrectSize) {
    reset();

    process_input("10");
    process_input("dict");
    process_input("begin");

    process_input("length");

    int size = std::get<int>(op_stack.back());
    std::cout << "Size: " << size << "\n";
    EXPECT_EQ(size, 0);
}

TEST(DictionaryOps, BeginPushesDictionaryStack) {
    reset();

    process_input("10");
    process_input("dict");
    process_input("begin");

    EXPECT_EQ(dict_stack.size(), 2);
}

TEST(DictionaryOps, EndPopsDictionaryStack) {
    reset();

    process_input("10");
    process_input("dict");
    process_input("begin");
    process_input("end");

    EXPECT_EQ(dict_stack.size(), 1);
}

TEST(DictionaryOps, DefStoresValue) {
    reset();

    process_input("10");
    process_input("dict");
    process_input("begin");

    process_input("/x");
    process_input("42");
    process_input("def");

    process_input("x");

    ASSERT_FALSE(op_stack.empty());
    EXPECT_EQ(std::get<int>(op_stack.back()), 42);
}

TEST(DictionaryOps, DefOverridesExistingValue) {
    reset();

    process_input("10");
    process_input("dict");
    process_input("begin");

    process_input("/x");
    process_input("10");
    process_input("def");

    process_input("/x");
    process_input("99");
    process_input("def");

    process_input("x");

    EXPECT_EQ(std::get<int>(op_stack.back()), 99);
}

TEST(DictionaryOps, DefInvalidKeyThrows) {
    reset();

    process_input("10");
    process_input("dict");
    process_input("begin");

    EXPECT_THROW(process_input("x"), TypeMismatch);
}

TEST(DictionaryOps, DictAndDefChain) {
    reset();

    process_input("10");
    process_input("dict");
    process_input("begin");

    process_input("/a");
    process_input("1");
    process_input("def");

    process_input("/b");
    process_input("2");
    process_input("def");

    process_input("a");
    EXPECT_EQ(std::get<int>(op_stack.back()), 1);

    process_input("b");
    EXPECT_EQ(std::get<int>(op_stack.back()), 2);
}