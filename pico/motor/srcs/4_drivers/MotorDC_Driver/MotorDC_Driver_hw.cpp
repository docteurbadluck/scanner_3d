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

static void init_pwm_pin(uint8_t pin, uint32_t &slice, uint32_t &channel)
{
    gpio_set_function(pin, GPIO_FUNC_PWM);
    slice   = pwm_gpio_to_slice_num(pin);
    channel = pwm_gpio_to_channel(pin);
}
}

MotorDC_Driver::MotorDC_Driver(const MotorDC_DriverConfig &cfg, const MotorDC_DriverPins &pins)
    : _cfg(cfg), _io{}
{
    uint32_t in1_slice, in1_ch, in2_slice, in2_ch;

    gpio_init(pins.btn_up_pin);
    gpio_set_dir(pins.btn_up_pin, GPIO_IN);
    gpio_init(pins.btn_down_pin);
    gpio_set_dir(pins.btn_down_pin, GPIO_IN);

    if (pins.buttons_active_low)
    {
        gpio_pull_up(pins.btn_up_pin);
        gpio_pull_up(pins.btn_down_pin);
    }
    else
    {
        gpio_pull_down(pins.btn_up_pin);
        gpio_pull_down(pins.btn_down_pin);
    }

    init_pwm_pin(pins.in1_pin, in1_slice, in1_ch);
    init_pwm_pin(pins.in2_pin, in2_slice, in2_ch);

    pwm_set_wrap(in1_slice, 1000);
    pwm_set_chan_level(in1_slice, in1_ch, 0);
    pwm_set_enabled(in1_slice, true);

    if (in2_slice != in1_slice)
    {
        pwm_set_wrap(in2_slice, 1000);
        pwm_set_chan_level(in2_slice, in2_ch, 0);
        pwm_set_enabled(in2_slice, true);
    }
    else
    {
        pwm_set_chan_level(in2_slice, in2_ch, 0);
    }

    _io.drive_up = [in1_slice, in1_ch, in2_slice, in2_ch](uint8_t speed) {
        pwm_set_chan_level(in2_slice, in2_ch, 0);
        pwm_set_chan_level(in1_slice, in1_ch, speed_to_level(speed));
    };
    _io.drive_down = [in1_slice, in1_ch, in2_slice, in2_ch](uint8_t speed) {
        pwm_set_chan_level(in1_slice, in1_ch, 0);
        pwm_set_chan_level(in2_slice, in2_ch, speed_to_level(speed));
    };
    _io.stop = [in1_slice, in1_ch, in2_slice, in2_ch]() {
        pwm_set_chan_level(in1_slice, in1_ch, 0);
        pwm_set_chan_level(in2_slice, in2_ch, 0);
    };

    const bool active_low = pins.buttons_active_low;
    const uint8_t up_pin  = pins.btn_up_pin;
    const uint8_t dn_pin  = pins.btn_down_pin;

    _io.is_up_pressed = [up_pin, active_low]() {
        const bool raw = gpio_get(up_pin);
        return active_low ? !raw : raw;
    };
    _io.is_down_pressed = [dn_pin, active_low]() {
        const bool raw = gpio_get(dn_pin);
        return active_low ? !raw : raw;
    };
    _io.now_ms = []() {
        return to_ms_since_boot(get_absolute_time());
    };
    _io.sleep_ms = [](uint32_t ms) {
        ::sleep_ms(ms);
    };
}
