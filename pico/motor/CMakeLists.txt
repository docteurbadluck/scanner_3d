cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(motor C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(motor 
 srcs/main.cpp
)
target_link_libraries(motor pico_stdlib hardware_pwm hardware_spi hardware_i2c)
pico_enable_stdio_usb(motor 1)
pico_enable_stdio_uart(motor 0)
pico_add_extra_outputs(motor)