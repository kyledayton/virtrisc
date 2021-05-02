cmake_minimum_required(VERSION 3.19)
project(virtmachine C)

set(CMAKE_C_STANDARD 99)

set(
        SOURCES
        example.c
        display.c
)

add_executable(virtmachine ${SOURCES})
target_link_libraries(virtmachine PRIVATE virtrisc)