#include "StepperMotor_Driver.hpp"

#include <hardware/gpio.h>
#include <pico/stdlib.h>

void StepperMotor_Driver::_pulseStep()
{
    gpio_put(_pins.step_pin, true);
    ::sleep_us(_cfg.step_pulse_us);
    gpio_put(_pins.step_pin, false);
}
