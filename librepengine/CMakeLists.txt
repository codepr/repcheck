cmake_minimum_required(VERSION 3.18.1)

project(RepEngine)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add C library sources
set(REP_ENGINE_SOURCES
    src/rep_engine.c
    src/angle.c
    src/state_machine.c
    src/velocity.c
    src/rpe.c
    src/form.c
    src/log.c
)

# Create static library
add_library(repengine STATIC
    ${REP_ENGINE_SOURCES}
)

# Include directories
target_include_directories(repengine PUBLIC
    ${CMAKE_SOURCE_DIR}
)

# Compiler flags
target_compile_options(repengine PRIVATE
    -Wall
    -Wextra
    -O2
)

# Build tests if needed
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    
    # Helper function to create test executables
    function(add_repengine_test test_name)
        add_executable(${test_name} tests/${test_name}.c)
        target_link_libraries(${test_name} repengine m)
        add_test(NAME ${test_name} COMMAND ${test_name})
    endfunction()
    
    add_repengine_test(test_log)
    add_repengine_test(test_angle)
    add_repengine_test(test_state_machine)
    add_repengine_test(test_velocity)
    add_repengine_test(test_rpe)
    add_repengine_test(test_form)
    add_repengine_test(test_rep_engine)
endif()
