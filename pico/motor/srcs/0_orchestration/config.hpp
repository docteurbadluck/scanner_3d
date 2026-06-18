#pragma once

#include "4_drivers/ServoMotor_Driver/ServoMotor_Driver.hpp"
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

// ── Servo MG995  (GP21, GP26) ───────────────────────────────────────────────
//   PWM : GP21 (PWM2 B, pin 27)
//   Shunt 0.5Ω sur ligne VCC servo → GP26 (ADC0, physical pin 31)
inline const ServoMotor_DriverPins SERVO_PINS
{
    21,      // pwm_pin  (GP21 = PWM2 B, pin 27)
    26,      // adc_pin  (GP26 = ADC0,   pin 31)
};

inline const ServoMotor_DriverConfig SERVO_CONFIG
{
    0.0f,    // angle_up_a_deg
    45.0f,   // angle_up_b_deg
    90.0f,   // angle_down_a_deg
    135.0f,  // angle_down_b_deg
    400,     // move_delay_ms
    80,      // current_threshold_adc (~65mV sur 3.3V/12bit)
    0.5f,    // shunt_ohms  (deux 1Ω en parallèle)
};

// ── DC Motor JGB37  (GP6, GP8, GP16, GP18, GP27, GP28) ─────────────────────
//   IN1 PWM : GP6  (PWM slice 3A)
//   IN2 PWM : GP8  (PWM slice 4A)
//   BTN_UP  : GP16 (active-low endstop, pin 21)
//   BTN_DOWN: GP18 (active-low endstop, pin 24)
//   R_IS1   : GP28 (ADC2, pin 34) — chip 1, sens montée
//   R_IS2   : GP27 (ADC1, pin 32) — chip 2, sens descente
inline const MotorDC_DriverPins MOTOR_DC_PINS
{
    6,       // in1_pin
    8,       // in2_pin
    16,      // btn_up_pin
    18,      // btn_down_pin
    true,    // buttons_active_low
    28,      // adc_pin   (R_IS1 → GP28 = ADC2, pin 34)
    27,      // adc_pin_2 (R_IS2 → GP27 = ADC1, pin 32)
};

inline const MotorDC_DriverConfig MOTOR_DC_CONFIG
{
    60,      // speed_percent
    3000,    // timeout_ms
    1,       // poll_interval_ms
    300,     // stall_threshold_adc (2.2kΩ, calibrer entre ~96 libre et ~1600 bloqué)
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
    5000,    // step_delay_us
};
