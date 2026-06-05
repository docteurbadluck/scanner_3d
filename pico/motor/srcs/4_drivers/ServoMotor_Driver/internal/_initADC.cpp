#include "ServoMotor_Driver.hpp"

#include <hardware/adc.h>

void ServoMotor_Driver::_initADC()
{
    adc_init();
    adc_gpio_init(_pins.adc_pin);
    _adc_channel = _pins.adc_pin - 26u;
}
