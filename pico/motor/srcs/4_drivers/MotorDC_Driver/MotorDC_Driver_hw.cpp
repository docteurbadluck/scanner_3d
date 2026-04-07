#include "MotorDC_Driver.hpp"

#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include <pico/time.h>

#include <algorithm>

namespace
{
struct PwmPins { uint32_t s1, c1, s2, c2; };

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

static void set_button_pull(uint8_t pin, bool active_low)
{
    if (active_low)
        gpio_pull_up(pin);
    else
        gpio_pull_down(pin);
}

static void init_buttons(const MotorDC_DriverPins &pins)
{
    gpio_init(pins.btn_up_pin);
    gpio_set_dir(pins.btn_up_pin, GPIO_IN);
    set_button_pull(pins.btn_up_pin, pins.buttons_active_low);
    gpio_init(pins.btn_down_pin);
    gpio_set_dir(pins.btn_down_pin, GPIO_IN);
    set_button_pull(pins.btn_down_pin, pins.buttons_active_low);
}

static void init_second_pwm_slice(uint32_t s1, PwmPins &pw)
{
    if (pw.s2 != s1)
    {
        pwm_set_wrap(pw.s2, 1000);
        pwm_set_chan_level(pw.s2, pw.c2, 0);
        pwm_set_enabled(pw.s2, true);
    }
    else
        pwm_set_chan_level(pw.s2, pw.c2, 0);
}

static void init_pwm_slices(uint8_t in1, uint8_t in2, PwmPins &pw)
{
    init_pwm_pin(in1, pw.s1, pw.c1);
    init_pwm_pin(in2, pw.s2, pw.c2);
    pwm_set_wrap(pw.s1, 1000);
    pwm_set_chan_level(pw.s1, pw.c1, 0);
    pwm_set_enabled(pw.s1, true);
    init_second_pwm_slice(pw.s1, pw);
}

static void assign_drive_lambdas(MotorDC_DriverIO &io, PwmPins pw)
{
    io.drive_up = [pw](uint8_t speed) {
        pwm_set_chan_level(pw.s2, pw.c2, 0);
        pwm_set_chan_level(pw.s1, pw.c1, speed_to_level(speed));
    };
    io.drive_down = [pw](uint8_t speed) {
        pwm_set_chan_level(pw.s1, pw.c1, 0);
        pwm_set_chan_level(pw.s2, pw.c2, speed_to_level(speed));
    };
    io.stop = [pw]() {
        pwm_set_chan_level(pw.s1, pw.c1, 0);
        pwm_set_chan_level(pw.s2, pw.c2, 0);
    };
}

static void assign_button_lambdas(MotorDC_DriverIO &io, const MotorDC_DriverPins &pins)
{
    const bool    active_low = pins.buttons_active_low;
    const uint8_t up_pin     = pins.btn_up_pin;
    const uint8_t dn_pin     = pins.btn_down_pin;
    io.is_up_pressed   = [up_pin, active_low]() {
        const bool raw = gpio_get(up_pin);
        return active_low ? !raw : raw;
    };
    io.is_down_pressed = [dn_pin, active_low]() {
        const bool raw = gpio_get(dn_pin);
        return active_low ? !raw : raw;
    };
}
}

MotorDC_Driver::MotorDC_Driver(const MotorDC_DriverConfig &cfg, const MotorDC_DriverPins &pins)
    : _cfg(cfg), _io{}
{
    PwmPins pw;
    init_buttons(pins);
    init_pwm_slices(pins.in1_pin, pins.in2_pin, pw);
    assign_drive_lambdas(_io, pw);
    assign_button_lambdas(_io, pins);
    _io.now_ms   = []() { return to_ms_since_boot(get_absolute_time()); };
    _io.sleep_ms = [](uint32_t ms) { ::sleep_ms(ms); };
}
