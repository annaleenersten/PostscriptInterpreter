#include <gtest/gtest.h>
#include "parser.h"
#include "interpreter.h"

extern void process_input(const std::string& input);
extern std::vector<Value> op_stack;
extern std::vector<PSDict*> dict_stack;
extern void init_interpreter();

extern void reset();

// ---------- Dictionary Tests ----------

TEST(DictionaryOps, DictCreatesEmptyDictionary) {
    reset();

    process_input("10");
    process_input("dict");

    EXPECT_TRUE(std::holds_alternative<PSDict*>(op_stack.back()));
    PSDict* d = std::get<PSDict*>(op_stack.back());

    EXPECT_EQ(d->dict.size(), 0);
}

TEST(DictionaryOps, LengthReturnsCorrectSize) {
    reset();

    process_input("/x 10 def");   // add entry
    process_input("dict");        // push dict (may be ignored depending on design)
    process_input("length");

    int size = std::get<int>(op_stack.back());
    EXPECT_EQ(size, 1);
}

TEST(DictionaryOps, MaxLengthReturnsValue) {
    reset();

    process_input("10 dict");
    process_input("maxlength");

    int cap = std::get<int>(op_stack.back());

    EXPECT_EQ(cap, 0);  // or size depending on your implementation
}

TEST(DictionaryOps, BeginPushesDictionaryStack) {
    reset();

    process_input("10 dict");
    process_input("begin");

    EXPECT_GE(dict_stack.size(), 2);
}

TEST(DictionaryOps, EndPopsDictionaryStack) {
    reset();

    process_input("10 dict");
    process_input("begin");
    process_input("end");

    EXPECT_EQ(dict_stack.size(), 1);
}

TEST(DictionaryOps, DefStoresValue) {
    reset();

    process_input("/x 42 def");

    // lookup
    process_input("x");

    EXPECT_EQ(std::get<int>(op_stack.back()), 42);
}

TEST(DictionaryOps, DefOverridesExistingValue) {
    reset();

    process_input("/x 10 def");
    process_input("/x 99 def");

    process_input("x");

    EXPECT_EQ(std::get<int>(op_stack.back()), 99);
}

TEST(DictionaryOps, DefInvalidKeyThrows) {
    reset();

    process_input("x");     // invalid key (no '/')
    process_input("10");

    EXPECT_THROW(process_input("def"), TypeMismatch);
}