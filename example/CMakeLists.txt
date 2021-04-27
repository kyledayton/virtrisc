cmake_minimum_required(VERSION 3.19)
project(virtrisc_example C)

set(CMAKE_C_STANDARD 99)

set(
        SOURCES
        example.c
)

add_executable(virtrisc_example ${SOURCES})
target_link_libraries(virtrisc_example PRIVATE virtrisc)