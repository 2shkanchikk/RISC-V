cmake_minimum_required(VERSION 3.16)
project(riscv CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(riscv
    src/main.cpp
    src/core/bus.cpp
    src/core/cpu.cpp
    src/core/ram.cpp
    src/loader/loader.cpp
    src/devices/uart.cpp
    src/devices/display.cpp
    src/devices/timer.cpp
    src/platform/platform.cpp
)

target_include_directories(riscv PRIVATE src)
target_compile_options(riscv PRIVATE -Wall -Wextra)