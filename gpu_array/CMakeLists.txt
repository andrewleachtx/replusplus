add_library(gpu_array INTERFACE)
target_include_directories(gpu_array
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
add_executable(gpu_array_tests gpu_array.cu)

target_link_libraries(gpu_array_tests PRIVATE
    gpu_array common gtest gtest_main
)
# target_compile_options(gpu_array_tests PRIVATE
#     $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler=-Wall,-Wextra>
# )
add_test(NAME gpu_array_test COMMAND gpu_array_tests)
