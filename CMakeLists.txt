cmake_minimum_required(VERSION 3.20)
project(replusplus LANGUAGES CXX CUDA)

# Add Google Test
add_subdirectory(vendor/googletest)
enable_testing()

add_subdirectory(common)
add_subdirectory(vector)
add_subdirectory(gpu_array)
add_subdirectory(systemc)
add_subdirectory(variant)
add_subdirectory(optional)
