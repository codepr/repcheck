cmake_minimum_required(VERSION 3.22.1)

project(RepCount)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add C library sources from librepengine
set(REP_ENGINE_SOURCES
    ${CMAKE_SOURCE_DIR}/../librepengine/src/rep_engine.c
    ${CMAKE_SOURCE_DIR}/../librepengine/src/angle.c
    ${CMAKE_SOURCE_DIR}/../librepengine/src/state_machine.c
    ${CMAKE_SOURCE_DIR}/../librepengine/src/velocity.c
    ${CMAKE_SOURCE_DIR}/../librepengine/src/rpe.c
    ${CMAKE_SOURCE_DIR}/../librepengine/src/form.c
    ${CMAKE_SOURCE_DIR}/../librepengine/src/log.c
)

# Add JNI source
set(JNI_SOURCES
    ${CMAKE_SOURCE_DIR}/app/src/main/jni/repengine_jni.cpp
)

# Create shared library with C engine + JNI
add_library(repengine SHARED
    ${REP_ENGINE_SOURCES}
    ${JNI_SOURCES}
)

# Include directories
target_include_directories(repengine PRIVATE
    ${CMAKE_SOURCE_DIR}/../librepengine/src
    ${CMAKE_SOURCE_DIR}/app/src/main/jni
)

# Compiler flags
target_compile_options(repengine PRIVATE
    -Wall
    -Wextra
    -O2
)

# Link standard libraries
target_link_libraries(repengine
    log
)
