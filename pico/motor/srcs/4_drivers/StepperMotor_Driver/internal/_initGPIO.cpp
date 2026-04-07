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

    if (_pins.has_enable)
    {
        gpio_init(_pins.enable_pin);
        gpio_set_dir(_pins.enable_pin, GPIO_OUT);
        gpio_put(_pins.enable_pin, true);
    }
}
