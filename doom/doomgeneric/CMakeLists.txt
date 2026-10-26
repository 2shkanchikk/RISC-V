cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/riscv.cmake)
project(doom C)

set(CMAKE_BUILD_TYPE Release)

set(DOOM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doomgeneric)
set(SDK_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/../../sdk)

# все .c игры, кроме чужих портов и звука через SDL/Allegro
file(GLOB DOOM_SOURCES ${DOOM_DIR}/*.c)
list(FILTER DOOM_SOURCES EXCLUDE REGEX ".*/doomgeneric_.*\\.c$")
list(FILTER DOOM_SOURCES EXCLUDE REGEX ".*/i_(sdl|allegro).*\\.c$")
list(APPEND DOOM_SOURCES ${DOOM_DIR}/doomgeneric_riscv.c)   # наш порт

add_executable(doom ${DOOM_SOURCES} ${SDK_DIR}/picolibc.c)
target_include_directories(doom PRIVATE ${DOOM_DIR} ${SDK_DIR})
target_compile_definitions(doom PRIVATE DOOMGENERIC_RESX=320 DOOMGENERIC_RESY=200)
target_compile_options(doom PRIVATE -w)   # не показывать сотни предупреждений из кода Doom
set_target_properties(doom PROPERTIES SUFFIX ".elf")

add_custom_command(TARGET doom POST_BUILD
    COMMAND riscv64-unknown-elf-objcopy -O binary
            $<TARGET_FILE:doom> $<TARGET_FILE_DIR:doom>/doom.bin)