#include "StepperMotor_Driver.hpp"

#include <hardware/gpio.h>

void StepperMotor_Driver::_initGPIO()
{
    gpio_init(_pins.step_pin);
    gpio_set_dir(_pins.step_pin, GPIO_OUT);
    gpio_put(_pins.step_pin, false);

    gpio_init(_pins.dir_pin);
    gpio_set_dir(_pins.dir_pin, GPIO_OUT);
    gpio_put(_pins.dir_pin, false);
}
