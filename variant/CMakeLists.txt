add_library(variant INTERFACE)
target_include_directories(variant
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
add_executable(variant_tests variant.cpp)

# We only need to link gtest and variant.hpp to our executable variant.cpp
# so PRIVATE because inheritors of variant don't need it
target_link_libraries(variant_tests PRIVATE
    variant gtest gtest_main
)
target_compile_options(variant_tests PRIVATE
    -Wall
    -Wextra
    -Werror
    -pedantic
)
add_test(NAME variant_test COMMAND variant_tests)
