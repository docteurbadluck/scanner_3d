#include "StepperMotor_Driver.hpp"

#include <hardware/gpio.h>
#include <pico/stdlib.h>

bool StepperMotor_Driver::stepForward()
{
    gpio_put(_pins.dir_pin, true);
    ::sleep_us(_cfg.dir_setup_us);
    _pulseStep();
    ::sleep_us(_cfg.step_delay_us);
    return true;
}
