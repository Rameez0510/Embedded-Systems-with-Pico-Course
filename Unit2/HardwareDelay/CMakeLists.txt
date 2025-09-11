cmake_minimum_required(VERSION 3.13)

# Import the Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Initialize the Pico SDK
pico_sdk_init()

# Project settings
project(HardwareDelay C CXX ASM)     
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(HardwareDelayBlink HardwareDelayBlink.c)  
target_link_libraries(HardwareDelayBlink pico_stdlib)
pico_add_extra_outputs(HardwareDelayBlink)

add_executable(Blink_2_LEDs Blink_2_LEDs.c)  
target_link_libraries(Blink_2_LEDs pico_stdlib)
pico_add_extra_outputs(Blink_2_LEDs)

