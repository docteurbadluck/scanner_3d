#include "StepperMotor_Driver.hpp"

#include <hardware/gpio.h>

void StepperMotor_Driver::_enable()
{
    if (_pins.has_enable)
        gpio_put(_pins.enable_pin, false);
}
