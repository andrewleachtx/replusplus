cmake_minimum_required(VERSION 3.20)
project(replusplus LANGUAGES CXX)

# Add Google Test
add_subdirectory(vendor/googletest)
enable_testing()

add_subdirectory(vector)
