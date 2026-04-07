#pragma once

#include "4_drivers/ServoMotor_Driver/ServoMotor_Driver.hpp"
#include "4_drivers/Accelerometer_Driver/Accelerometer_Driver.hpp"
#include "4_drivers/MotorDC_Driver/MotorDC_Driver.hpp"
#include "4_drivers/StepperMotor_Driver/StepperMotor_Driver.hpp"
#include "4_drivers/UsbReceptor_Driver/UsbReceptor_Driver.hpp"
#include "4_drivers/UsbSender_Driver/UsbSender_Driver.hpp"

#include <hardware/i2c.h>

// ── USB CDC  (communication + flashing) ────────────────────────────────────
//   Pi ↔ Pico via /dev/ttyACM0  (stdio_usb, enabled in CMakeLists.txt)
//   Flashing via SWD (openocd, no BOOTSEL needed):
//     Pi GPIO 24 → Pico SWDCLK  (debug header pad)
//     Pi GPIO 25 → Pico SWDIO   (debug header pad)
//   GP0 and GP1 are free.
inline const UsbReceptor_DriverConfig USB_RECEPTOR_CONFIG
{
    .poll_timeout_us = 100,
    .delimiter       = '\n',
};

// ── Servo MG995  (GP2 — physical pin 4) ────────────────────────────────────
inline const ServoMotor_DriverPins SERVO_PINS
{
    .pwm_pin = 2,
};

inline const ServoMotor_DriverConfig SERVO_CONFIG
{
    .angle_up_a_deg   = 0.0f,
    .angle_up_b_deg   = 45.0f,
    .angle_down_a_deg = 90.0f,
    .angle_down_b_deg = 135.0f,
    .move_delay_ms    = 400,
};

// ── Accelerometers MPU-6050 × 2  (GP4 / GP5 — physical pins 6, 7) ──────────
//   Both sensors share the same I2C bus (i2c0).
//   Sensor #1 : AD0 pulled LOW  → address 0x68
//   Sensor #2 : AD0 pulled HIGH → address 0x69
inline const Accelerometer_DriverPins ACC_PINS
{
    .sda_pin = 4,
    .scl_pin = 5,
};

inline const Accelerometer_DriverConfig ACC1_CONFIG
{
    .i2c_port  = i2c0,
    .address   = 0x68,
    .baud_rate = 400000,
};

inline const Accelerometer_DriverConfig ACC2_CONFIG
{
    .i2c_port  = i2c0,
    .address   = 0x69,
    .baud_rate = 400000,
};

// ── DC Motor JGB37  (GP6–GP9 — physical pins 9–12) ─────────────────────────
//   IN1 PWM : GP6  (PWM slice 3A)
//   IN2 PWM : GP7  (PWM slice 3B)
//   BTN_UP  : GP8  (active-low endstop)
//   BTN_DOWN: GP9  (active-low endstop)
//   NOTE: MotorDC_Driver redesign pending — IN1/IN2 will become dual-PWM.
//         pwm_enable_pin is a temporary placeholder (GP3, unused pin).
inline const MotorDC_DriverPins MOTOR_DC_PINS
{
    .in1_pin            = 6,
    .in2_pin            = 7,
    .btn_up_pin         = 8,
    .btn_down_pin       = 9,
    .buttons_active_low = true,
};

inline const MotorDC_DriverConfig MOTOR_DC_CONFIG
{
    .speed_percent    = 60,
    .timeout_ms       = 3000,
    .poll_interval_ms = 1,
};

// ── Stepper NEMA23 + DM556  (GP10–GP12 — physical pins 14–16) ──────────────
//   STEP : GP10
//   DIR  : GP11
//   ENA  : GP12  (active-low)
inline const StepperMotor_DriverPins STEPPER_PINS
{
    .step_pin   = 10,
    .dir_pin    = 11,
    .enable_pin = 12,
    .has_enable = true,
};

inline const StepperMotor_DriverConfig STEPPER_CONFIG
{
    .dir_setup_us  = 10,
    .step_pulse_us = 10,
    .step_delay_us = 500,
};
