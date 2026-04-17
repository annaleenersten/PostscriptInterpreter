cmake_minimum_required(VERSION 3.14)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Enable testing
enable_testing()

# -------------------------
# Main library (your interpreter)
# -------------------------
add_library(my_lib
    src/parser.cpp
    src/interpreter.cpp
    src/psdict.cpp
    src/operations.cpp
    src/value.cpp
)

add_executable(app
    src/main.cpp
)

target_link_libraries(app
    my_lib
)

# IMPORTANT: headers live in /include
target_include_directories(my_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -------------------------
# GoogleTest setup
# -------------------------
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# Prevent GoogleTest from overriding your compiler settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# -------------------------
# Test executable
# -------------------------
add_executable(runTests
    tests/testParse.cpp
    tests/testStackManipulation.cpp
    tests/testArithmetic.cpp
    tests/testDictionary.cpp
    tests/testStrings.cpp
)

target_link_libraries(runTests
    PRIVATE
        my_lib
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(runTests)