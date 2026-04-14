cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(motor C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

file(GLOB_RECURSE MOTOR_SOURCES
    srcs/1_domain/*.cpp
    srcs/2_usecases/*.cpp
    srcs/3_interface/*.cpp
    srcs/4_drivers/*.cpp
)

add_executable(motor
    srcs/main.cpp
    ${MOTOR_SOURCES}
)
target_include_directories(motor PRIVATE
    srcs
    srcs/4_drivers/Accelerometer_Driver
    srcs/4_drivers/MotorDC_Driver
    srcs/4_drivers/ServoMotor_Driver
    srcs/4_drivers/StepperMotor_Driver
    srcs/4_drivers/UsbReceptor_Driver
    srcs/4_drivers/UsbSender_Driver
)
target_link_libraries(motor pico_stdlib hardware_pwm hardware_spi hardware_i2c)
pico_enable_stdio_usb(motor 1)
pico_enable_stdio_uart(motor 1)
pico_add_extra_outputs(motor)