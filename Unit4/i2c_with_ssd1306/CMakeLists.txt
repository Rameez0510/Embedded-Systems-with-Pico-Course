cmake_minimum_required(VERSION 3.13)

# Import the Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Initialize the Pico SDK
pico_sdk_init()

# Project settings
project(i2c_with_ssd1306 C CXX ASM)     
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# 01_initial_set_up
# add_executable(01_initial_set_up 01_initial_set_up.c)  
# target_link_libraries(01_initial_set_up pico_stdlib hardware_i2c) # add hardware_i2c
# pico_add_extra_outputs(01_initial_set_up)

# 02_i2c_scanner
# add_executable(02_i2c_scanner 02_i2c_scanner.c)  
# target_link_libraries(02_i2c_scanner pico_stdlib hardware_i2c)
# pico_enable_stdio_usb(02_i2c_scanner 1)   # enable USB serial
# pico_enable_stdio_uart(02_i2c_scanner 0)  # disable UART serial
# pico_add_extra_outputs(02_i2c_scanner)

# 03_turn_display_on_off
# add_executable(03_turn_display_on_off 03_turn_display_on_off.c)  
# target_link_libraries(03_turn_display_on_off pico_stdlib hardware_i2c)
# pico_add_extra_outputs(03_turn_display_on_off)

# 05_draw_line
# add_executable(05_draw_line 05_draw_line.c)  
# target_link_libraries(05_draw_line pico_stdlib hardware_i2c)
# pico_add_extra_outputs(05_draw_line)

# 06_draw_x_y
# add_executable(06_draw_x_y 06_draw_x_y.c)  
# target_link_libraries(06_draw_x_y pico_stdlib hardware_i2c)
# pico_add_extra_outputs(06_draw_x_y)

# 07_drawing_figures
# add_executable(07_drawing_figures 07_drawing_figures.c)  
# target_link_libraries(07_drawing_figures pico_stdlib hardware_i2c)
# pico_add_extra_outputs(07_drawing_figures)

# 08_display_text
# add_executable(08_display_text 08_display_text.c)  
# target_link_libraries(08_display_text pico_stdlib hardware_i2c)
# pico_add_extra_outputs(08_display_text)

# # 09_animation
# add_executable(09_animation 09_animation.c)  
# target_link_libraries(09_animation pico_stdlib hardware_i2c)
# pico_add_extra_outputs(09_animation)

# # 10_display_images
# add_executable(10_display_images 10_display_images.c)  
# target_link_libraries(10_display_images pico_stdlib hardware_i2c)
# pico_add_extra_outputs(10_display_images)

# 11_using_ssd1306_library
add_executable(11_using_ssd1306_library 11_using_ssd1306_library.c)  
target_link_libraries(11_using_ssd1306_library pico_stdlib hardware_i2c)
pico_add_extra_outputs(11_using_ssd1306_library)

