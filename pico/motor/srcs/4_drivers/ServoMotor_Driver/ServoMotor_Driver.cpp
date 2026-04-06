#include "ServoMotor_Driver.hpp"

#include <hardware/gpio.h>
#include <pico/stdlib.h>

static constexpr float    PULSE_MIN_US  = 500.0f;
static constexpr float    PULSE_RANGE   = 2000.0f;
static constexpr float    ANGLE_MAX_DEG = 180.0f;
static constexpr uint32_t PWM_WRAP      = 20000;
static constexpr float    PWM_CLKDIV    = 125.0f;

ServoMotor_Driver::ServoMotor_Driver(const ServoMotor_DriverConfig &cfg,
                                     const ServoMotor_DriverPins   &pins)
    : _cfg(cfg), _pins(pins), _pwm_slice(0), _pwm_channel(0)
{
    _initPWM();
}

void ServoMotor_Driver::_initPWM()
{
    gpio_set_function(_pins.pwm_pin, GPIO_FUNC_PWM);
    _pwm_slice   = pwm_gpio_to_slice_num(_pins.pwm_pin);
    _pwm_channel = pwm_gpio_to_channel(_pins.pwm_pin);
    pwm_set_clkdiv(_pwm_slice, PWM_CLKDIV);
    pwm_set_wrap(_pwm_slice, PWM_WRAP);
    pwm_set_chan_level(_pwm_slice, _pwm_channel, 0);
    pwm_set_enabled(_pwm_slice, true);
}

void ServoMotor_Driver::_setPulse(uint16_t pulse_us)
{
    pwm_set_chan_level(_pwm_slice, _pwm_channel, pulse_us);
}

float ServoMotor_Driver::_angleForPos(Pos_hand pos) const
{
    if (pos == Pos_hand::UP_A)   return _cfg.angle_up_a_deg;
    if (pos == Pos_hand::UP_B)   return _cfg.angle_up_b_deg;
    if (pos == Pos_hand::DOWN_A) return _cfg.angle_down_a_deg;
    return _cfg.angle_down_b_deg;
}

uint16_t ServoMotor_Driver::_angleToPulse(float angle_deg) const
{
    return static_cast<uint16_t>(PULSE_MIN_US + (angle_deg / ANGLE_MAX_DEG) * PULSE_RANGE);
}

bool ServoMotor_Driver::goInitialPos()
{
    return goTo(Pos_hand::UP_A);
}

bool ServoMotor_Driver::goTo(Pos_hand pos)
{
    if (pos == Pos_hand::UNKNOWN)
        return false;

    const float    angle = _angleForPos(pos);
    const uint16_t pulse = _angleToPulse(angle);
    _setPulse(pulse);
    ::sleep_ms(_cfg.move_delay_ms);
    return true;
}
