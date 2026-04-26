#pragma once

#include "4_drivers/ServoMotor_Driver/ServoMotor_Driver.hpp"
#include "4_drivers/Accelerometer_Driver/Accelerometer_Driver.hpp"
#include "4_drivers/MotorDC_Driver/MotorDC_Driver.hpp"
#include "4_drivers/StepperMotor_Driver/StepperMotor_Driver.hpp"
#include "4_drivers/UartReceptor_Driver/UartReceptor_Driver.hpp"
#include "4_drivers/UartSender_Driver/UartSender_Driver.hpp"

#include <hardware/i2c.h>

// ── UART0  (Pi ↔ Pico communication) ───────────────────────────────────────
//   GP0 = TX (physical pin 1)
//   GP1 = RX (physical pin 2)
//   Baud rate configured via pico-sdk stdio_uart (115200)
inline const UartReceptor_DriverConfig UART_RECEPTOR_CONFIG
{
    100,     // poll_timeout_us
    '\n',    // delimiter
    512,     // max_msg_size
};

// ── Servo MG995  (GP2 — physical pin 4) ────────────────────────────────────
inline const ServoMotor_DriverPins SERVO_PINS
{
    2,       // pwm_pin
};

inline const ServoMotor_DriverConfig SERVO_CONFIG
{
    0.0f,    // angle_up_a_deg
    45.0f,   // angle_up_b_deg
    90.0f,   // angle_down_a_deg
    135.0f,  // angle_down_b_deg
    400,     // move_delay_ms
};

// ── Accelerometers MPU-6050 × 2  (GP4 / GP5 — physical pins 6, 7) ──────────
//   Both sensors share the same I2C bus (i2c0).
//   Sensor #1 : AD0 pulled LOW  → address 0x68
//   Sensor #2 : AD0 pulled HIGH → address 0x69
inline const Accelerometer_DriverPins ACC_PINS
{
    4,       // sda_pin
    5,       // scl_pin
};

inline const Accelerometer_DriverConfig ACC1_CONFIG
{
    i2c0,    // i2c_port
    0x68,    // address
    400000,  // baud_rate
};

inline const Accelerometer_DriverConfig ACC2_CONFIG
{
    i2c0,    // i2c_port
    0x69,    // address
    400000,  // baud_rate
};

// ── DC Motor JGB37  (GP6–GP9 — physical pins 9–12) ─────────────────────────
//   IN1 PWM : GP6  (PWM slice 3A)
//   IN2 PWM : GP7  (PWM slice 3B)
//   BTN_UP  : GP8  (active-low endstop)
//   BTN_DOWN: GP9  (active-low endstop)
inline const MotorDC_DriverPins MOTOR_DC_PINS
{
    6,       // in1_pin
    7,       // in2_pin
    8,       // btn_up_pin
    9,       // btn_down_pin
    true,    // buttons_active_low
};

inline const MotorDC_DriverConfig MOTOR_DC_CONFIG
{
    60,      // speed_percent
    3000,    // timeout_ms
    1,       // poll_interval_ms
};

// ── Stepper NEMA23 + DM556  (GP10–GP12 — physical pins 14–16) ──────────────
//   STEP : GP10
//   DIR  : GP11
//   ENA  : GP12  (active-low)
inline const StepperMotor_DriverPins STEPPER_PINS
{
    10,      // step_pin
    11,      // dir_pin
    12,      // enable_pin
    true,    // has_enable
};

inline const StepperMotor_DriverConfig STEPPER_CONFIG
{
    10,      // dir_setup_us
    10,      // step_pulse_us
    500,     // step_delay_us
};
