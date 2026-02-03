find_package(PkgConfig REQUIRED)
pkg_check_modules(SYSTEMC REQUIRED systemc)

function(add_systemc_exercise NAME)
    add_executable(${NAME} main.cpp)
    target_include_directories(${NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${SYSTEMC_INCLUDE_DIRS}
    )
    target_link_libraries(${NAME} PRIVATE ${SYSTEMC_LIBRARIES})
    target_compile_options(${NAME} PRIVATE -Wall -Wextra)
endfunction()

add_subdirectory(01_hello_world)
add_subdirectory(02_dff)
add_subdirectory(03_counter)
add_subdirectory(04_adder)
add_subdirectory(05_shift_register)
add_subdirectory(06_fifo)
add_subdirectory(07_dma)
add_subdirectory(08_cache)
