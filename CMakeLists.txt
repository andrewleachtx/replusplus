cmake_minimum_required(VERSION 3.20)
project(replusplus LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

# Locate ccache
find_program(CCACHE_PROG ccache)
if(CCACHE_PROG)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROG})
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
else()
    message(STATUS "'ccache' not found, install for a (maybe) faster build")
endif()

add_library(replusplus INTERFACE)
target_include_directories(replusplus INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(vendor/googletest)
enable_testing()

add_executable(runTests tests/UniquePtr.cpp tests/WeakPtr.cpp tests/SharedPtr.cpp)
target_link_libraries(runTests PRIVATE replusplus gtest_main)
target_include_directories(runTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(runTests PRIVATE -fsanitize=address,leak,undefined -fno-omit-frame-pointer)
target_link_libraries(runTests PRIVATE -fsanitize=address,leak,undefined)

include(GoogleTest)
gtest_discover_tests(runTests)