cmake_minimum_required(VERSION 3.19)
project(virtrisc C)

set(CMAKE_C_STANDARD 99)

set(
        SOURCES
        src/cpu.c
        src/machine.c
        src/memory.c
)
add_library(virtrisc ${SOURCES})
target_include_directories(virtrisc PUBLIC include)

add_subdirectory(example)