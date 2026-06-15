#include "StepperMotor_Driver.hpp"

#include <hardware/gpio.h>
#include <pico/stdlib.h>

void StepperMotor_Driver::_stepBackward()
{
    gpio_put(_pins.dir_pin, false);
    ::sleep_us(_cfg.dir_setup_us);
    _pulseStep();
    ::sleep_us(_cfg.step_delay_us);
}
