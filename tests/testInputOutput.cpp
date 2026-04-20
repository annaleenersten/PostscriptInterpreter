#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include "parser.h"
#include "interpreter.h"

extern void process_input(const std::string& input);
extern std::vector<Value> op_stack;
extern std::vector<PSDict*> dict_stack;
extern void reset();

// ================================================
//  INPUT/OUTPUT Operation Tests
// ================================================


// Helper: Capture std::cout output
class CaptureCout {
public:
    std::stringstream buffer;
    std::streambuf* old;

    CaptureCout() {
        old = std::cout.rdbuf(buffer.rdbuf());
    }

    ~CaptureCout() {
        std::cout.rdbuf(old);
    }

    std::string str() {
        return buffer.str();
    }
};


TEST(OutputOps, PrintNumber) {
    reset();
    CaptureCout cap;

    process_input("42");
    process_input("print");

    EXPECT_EQ(cap.str(), "42\n");
}

TEST(OutputOps, PrintBoolean) {
    reset();
    CaptureCout cap;

    process_input("true");
    process_input("print");

    EXPECT_EQ(cap.str(), "true\n");
}

TEST(OutputOps, PrintString) {
    reset();
    CaptureCout cap;

    process_input("(hello)");
    process_input("print");

    EXPECT_EQ(cap.str(), "hello\n");
}

TEST(OutputOps, EqualsNumber) {
    reset();
    CaptureCout cap;

    process_input("42");
    process_input("=");

    EXPECT_EQ(cap.str(), "42\n");
}

TEST(OutputOps, EqualsBoolean) {
    reset();
    CaptureCout cap;

    process_input("true");
    process_input("=");

    EXPECT_EQ(cap.str(), "true\n");
}

TEST(OutputOps, EqualsString) {
    reset();
    CaptureCout cap;

    process_input("(world)");
    process_input("=");

    EXPECT_EQ(cap.str(), "world\n");
}

TEST(OutputOps, DoubleEqualsNumber) {
    reset();
    CaptureCout cap;

    process_input("42");
    process_input("==");

    EXPECT_EQ(cap.str(), "42\n");
}

TEST(OutputOps, DoubleEqualsBoolean) {
    reset();
    CaptureCout cap;

    process_input("true");
    process_input("==");

    EXPECT_EQ(cap.str(), "true\n");
}

TEST(OutputOps, DoubleEqualsString) {
    reset();
    CaptureCout cap;

    process_input("(hello)");
    process_input("==");

    EXPECT_EQ(cap.str(), "(hello)\n");
}

TEST(OutputOps, DoubleEqualsDict) {
    reset();
    CaptureCout cap;

    process_input("10");
    process_input("dict");
    process_input("==");

    EXPECT_EQ(cap.str(), "<<dict>>\n");
}

TEST(OutputOps, DoubleEqualsEmptyStack) {
    reset();
    CaptureCout cap;

    EXPECT_THROW(process_input("=="), TypeMismatch);
}