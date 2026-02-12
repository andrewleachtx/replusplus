add_library(optional INTERFACE)
target_include_directories(optional
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
add_executable(optional_tests optional.cpp)

target_link_libraries(optional_tests PRIVATE
    optional gtest gtest_main
)
target_compile_options(optional_tests PRIVATE
    -Wall
    -Wextra
    -Werror
    -pedantic
)
add_test(NAME optional_test COMMAND optional_tests)
