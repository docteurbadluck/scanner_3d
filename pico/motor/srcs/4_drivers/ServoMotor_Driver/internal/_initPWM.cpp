#include "ServoMotor_Driver.hpp"

#include <hardware/gpio.h>

namespace
{
static constexpr uint32_t PWM_WRAP   = 20000;
static constexpr float    PWM_CLKDIV = 125.0f;
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
