cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(blink_gp15 C CXX ASM)

pico_sdk_init()

add_executable(blink_gp15
    blink_gp15.c
)

target_link_libraries(blink_gp15 pico_stdlib)

pico_add_extra_outputs(blink_gp15)
