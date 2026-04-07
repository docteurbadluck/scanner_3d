#include "Accelerometer_Driver.hpp"

#include <hardware/gpio.h>
#include <hardware/i2c.h>

void Accelerometer_Driver::_initI2C()
{
    i2c_init(_cfg.i2c_port, _cfg.baud_rate);
    gpio_set_function(_pins.sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(_pins.scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(_pins.sda_pin);
    gpio_pull_up(_pins.scl_pin);
}
