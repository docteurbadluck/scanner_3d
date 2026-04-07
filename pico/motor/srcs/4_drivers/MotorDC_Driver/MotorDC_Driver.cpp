#include "MotorDC_Driver.hpp"

#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include <pico/time.h>

#include <algorithm>

namespace
{
static uint16_t speed_to_level(uint8_t speed_percent)
{
    const uint8_t clamped = std::min<uint8_t>(100U, speed_percent);
    return static_cast<uint16_t>(clamped * 10U);
}
}

MotorDC_Driver::MotorDC_Driver(const MotorDC_DriverConfig &cfg, const MotorDC_DriverPins &pins)
    : _cfg(cfg),
      _pins(pins),
      _in1_pwm_slice(0),
      _in1_pwm_channel(0),
      _in2_pwm_slice(0),
      _in2_pwm_channel(0)
{
    gpio_init(_pins.btn_up_pin);
    gpio_set_dir(_pins.btn_up_pin, GPIO_IN);

    gpio_init(_pins.btn_down_pin);
    gpio_set_dir(_pins.btn_down_pin, GPIO_IN);

    if (_pins.buttons_active_low)
    {
        gpio_pull_up(_pins.btn_up_pin);
        gpio_pull_up(_pins.btn_down_pin);
    }
    else
    {
        gpio_pull_down(_pins.btn_up_pin);
        gpio_pull_down(_pins.btn_down_pin);
    }

    gpio_set_function(_pins.in1_pin, GPIO_FUNC_PWM);
    gpio_set_function(_pins.in2_pin, GPIO_FUNC_PWM);

    _in1_pwm_slice = pwm_gpio_to_slice_num(_pins.in1_pin);
    _in1_pwm_channel = pwm_gpio_to_channel(_pins.in1_pin);
    _in2_pwm_slice = pwm_gpio_to_slice_num(_pins.in2_pin);
    _in2_pwm_channel = pwm_gpio_to_channel(_pins.in2_pin);

    pwm_set_wrap(_in1_pwm_slice, 1000);
    pwm_set_chan_level(_in1_pwm_slice, _in1_pwm_channel, 0);
    pwm_set_enabled(_in1_pwm_slice, true);

    if (_in2_pwm_slice != _in1_pwm_slice)
    {
        pwm_set_wrap(_in2_pwm_slice, 1000);
        pwm_set_chan_level(_in2_pwm_slice, _in2_pwm_channel, 0);
        pwm_set_enabled(_in2_pwm_slice, true);
    }
    else
    {
        pwm_set_chan_level(_in2_pwm_slice, _in2_pwm_channel, 0);
    }
}

void MotorDC_Driver::_drive_up()
{
    pwm_set_chan_level(_in2_pwm_slice, _in2_pwm_channel, 0);
    pwm_set_chan_level(_in1_pwm_slice, _in1_pwm_channel, speed_to_level(_cfg.speed_percent));
}

void MotorDC_Driver::_drive_down()
{
    pwm_set_chan_level(_in1_pwm_slice, _in1_pwm_channel, 0);
    pwm_set_chan_level(_in2_pwm_slice, _in2_pwm_channel, speed_to_level(_cfg.speed_percent));
}

void MotorDC_Driver::_stop()
{
    pwm_set_chan_level(_in1_pwm_slice, _in1_pwm_channel, 0);
    pwm_set_chan_level(_in2_pwm_slice, _in2_pwm_channel, 0);
}

bool MotorDC_Driver::_is_up_pressed() const
{
    const bool raw = gpio_get(_pins.btn_up_pin);
    return _pins.buttons_active_low ? !raw : raw;
}

bool MotorDC_Driver::_is_down_pressed() const
{
    const bool raw = gpio_get(_pins.btn_down_pin);
    return _pins.buttons_active_low ? !raw : raw;
}

bool MotorDC_Driver::_wait_until_pressed(Pos pos) const
{
    const uint32_t start = to_ms_since_boot(get_absolute_time());

    while ((to_ms_since_boot(get_absolute_time()) - start) <= _cfg.timeout_ms)
    {
        if (pos == Pos::UP && _is_up_pressed())
            return true;
        if (pos == Pos::DOWN && _is_down_pressed())
            return true;
        ::sleep_ms(_cfg.poll_interval_ms);
    }
    return false;
}

bool MotorDC_Driver::goInitialPos()
{
    return goTo(Pos::UP);
}

bool MotorDC_Driver::goTo(Pos pos)
{
    if (pos == Pos::UNKNOWN)
        return false;

    if (pos == Pos::UP)
    {
        if (_is_up_pressed())
        {
            _stop();
            return true;
        }

        _drive_up();
        const bool reached = _wait_until_pressed(Pos::UP);
        _stop();
        return reached;
    }

    if (_is_down_pressed())
    {
        _stop();
        return true;
    }

    _drive_down();
    const bool reached = _wait_until_pressed(Pos::DOWN);
    _stop();
    return reached;
}
