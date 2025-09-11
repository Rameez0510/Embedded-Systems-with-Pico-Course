cmake_minimum_required(VERSION 3.13)

# Import the Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Initialize the Pico SDK
pico_sdk_init()

# Project settings
project(button_interrupt C CXX ASM)     
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(button_Interrupt_no_debounce button_Interrupt_no_debounce.c)  
target_link_libraries(button_Interrupt_no_debounce pico_stdlib)
pico_add_extra_outputs(button_Interrupt_no_debounce)

add_executable(button_Interrupt_software_debounce button_Interrupt_software_debounce.c)  
target_link_libraries(button_Interrupt_software_debounce pico_stdlib)
pico_add_extra_outputs(button_Interrupt_software_debounce)

