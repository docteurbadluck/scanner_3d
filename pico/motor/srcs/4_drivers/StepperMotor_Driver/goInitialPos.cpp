#include "StepperMotor_Driver.hpp"

#include <hardware/gpio.h>

bool StepperMotor_Driver::goInitialPos()
{
    gpio_put(_pins.dir_pin, true);
    return true;
}
