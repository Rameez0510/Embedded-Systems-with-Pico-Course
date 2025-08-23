cmake_minimum_required(VERSION 3.13)

# Import the Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Initialize the Pico SDK
pico_sdk_init()

# Project settings
project(<ProjectName> C CXX ASM)      # Replace <ProjectName> with your project name
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


# Adding program(s)
add_executable(blink_led blink_led.c)  
target_link_libraries(blink_led pico_stdlib)
pico_add_extra_outputs(blink_led)

add_executable(read_push_button read_push_button.c) 
target_link_libraries(read_push_button pico_stdlib)

#Enabling serial Monitor
pico_enable_stdio_usb(read_push_button 1)   # enable USB serial
pico_enable_stdio_uart(read_push_button 0)  # disable UART serial

pico_add_extra_outputs(read_push_button)

