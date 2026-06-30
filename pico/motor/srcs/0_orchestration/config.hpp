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

// ── Servo MG995  (GP21, GP28) ───────────────────────────────────────────────
//   PWM : GP21 (PWM2 B, pin 27)
//   Shunt 0.5Ω sur ligne VCC servo → GP28
inline const ServoMotor_DriverPins SERVO_PINS
{
    21,      // pwm_pin  (GP21 = PWM2 B, pin 27)
    28,      // adc_pin  (GP28 = ADC2,   pin 34)
};

inline const ServoMotor_DriverConfig SERVO_CONFIG
{
    180.0f,    // angle_up_a_deg
    165.0f,   // angle_up_b_deg
    0.0f,   // angle_down_a_deg
    15.0f,  // angle_down_b_deg
    400,     // move_delay_ms
    80,      // current_threshold_adc (~65mV sur 3.3V/12bit)
    0.5f,    // shunt_ohms  (deux 1Ω en parallèle)
    90.0f,   // angle_safe_deg (milieu des 4 angles, passage sûr pour le bras)
};

// ── DC Motor JGB37  (GP6, GP8, GP17, GP18, GP27) ───────────────────────────
//   IN1 PWM : GP6  (PWM slice 3A)
//   IN2 PWM : GP8  (PWM slice 4A)
//   BTN_UP  : GP18 (active-low endstop, pin 24)
//   BTN_DOWN: GP17 (active-low endstop, pin 22)
//   ACS712  : GP26 (ADC1, pin 32) — OUT du module, en série sur l'alim moteur,
//             VCC=5V, sensibilité 185mV/A, sortie ~2.5V au repos
inline const MotorDC_DriverPins MOTOR_DC_PINS
{
    6,       // in1_pin
    8,       // in2_pin
    18,      // btn_up_pin
    17,      // btn_down_pin
    true,    // buttons_active_low
    26,      // acs712_pin (OUT → GP26)
};

inline const MotorDC_DriverConfig MOTOR_DC_CONFIG
{
    40,      // speed_percent
    3000,    // timeout_ms
    1,       // poll_interval_ms
    35,      // stall_threshold_adc (mesuré: bruit repos ±15-20, pic blocage ~41-55, à affiner)
    8,       // stall_debounce_polls (8 polls à 1ms = 8ms soutenus, filtre le bruit de commutation PWM)
    500,     // stall_grace_ms (laisse le temps de dépasser l'inertie/inrush au démarrage)
};

// ── Stepper NEMA23 + DM556  (GP10, GP12 — physical pins 14, 16) ────────────
//   STEP : GP10
//   DIR  : GP12
inline const StepperMotor_DriverPins STEPPER_PINS
{
    10,      // step_pin
    12,      // dir_pin
};

inline const StepperMotor_DriverConfig STEPPER_CONFIG
{
    10,      // dir_setup_us
    10,      // step_pulse_us
    15000,    // step_delay_us
};
