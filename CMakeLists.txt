cmake_minimum_required(VERSION 3.29)
project(replusplus LANGUAGES CXX)

# Extend replusplus as a library, interface means other targets depend on it, but CMake
# doesn't compile source files for it during the downstream build process
add_library(replusplus INTERFACE)
add_library(replusplus::replusplus ALIAS replusplus)

# Anything linking to replusplus gets -I/path/to/replusplus/include so now it can write
#include <replusplus/vector.hpp> instead of something like #include "../include/replusplus/vector.hpp"
target_include_directories(
    replusplus
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Anything consuming replusplus must be >=std=c++20
target_compile_features(
    replusplus
    INTERFACE
        cxx_std_20
)

enable_testing()

# Add Google Test
add_subdirectory(vendor/googletest)

# Make one executable that builds all .cpp tests in tests/
add_executable(
    replusplus_tests
    # tests/vector_tests.cpp
    tests/optional_tests.cpp
    tests/variant_tests.cpp
    tests/allocator_tests.cpp
)

# This executable doesn't know anything about replusplus or GoogleTest yet - so make it depend on both targets
# this gives us an entry point in gtest_main
target_link_libraries(
    replusplus_tests
    PRIVATE
        replusplus
        GTest::gtest_main
)

# CTest is enabled by enable_testing() above, but CTest doesn't know which GoogleTest tests we have instantiated inside of our tests/
# so include them
include(GoogleTest)
gtest_discover_tests(replusplus_tests)