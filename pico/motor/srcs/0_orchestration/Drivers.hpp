#pragma once

#include "0_orchestration/config.hpp"

struct Drivers
{
    UartReceptor_Driver  receptor;
    UartSender_Driver    uartSender;
    MotorDC_Driver       motorDC;
    ServoMotor_Driver    servo;
    StepperMotor_Driver  stepper;
    Accelerometer_Driver acc1;
    Accelerometer_Driver acc2;

    Drivers() :
        receptor(UART_RECEPTOR_CONFIG), uartSender(),
        motorDC(MOTOR_DC_CONFIG, MOTOR_DC_PINS),
        servo(SERVO_CONFIG, SERVO_PINS),
        stepper(STEPPER_CONFIG, STEPPER_PINS),
        acc1(ACC1_CONFIG, ACC_PINS), acc2(ACC2_CONFIG, ACC_PINS)
    {}
};
