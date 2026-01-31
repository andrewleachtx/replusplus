add_library(vector INTERFACE)
target_include_directories(vector
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
add_executable(vector_tests vector.cpp)

# We only need to link gtest and vector.hpp to our executable vector.cpp
# so PRIVATE because inheritors of vector don't need it
target_link_libraries(vector_tests PRIVATE
    vector gtest gtest_main
)
add_test(NAME vector_test COMMAND vector_tests)
