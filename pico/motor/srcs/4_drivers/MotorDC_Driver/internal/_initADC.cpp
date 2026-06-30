#include "../MotorDC_Driver.hpp"

#include <hardware/adc.h>

void MotorDC_Driver::_initADC(uint8_t acs712_pin)
{
    adc_init();
    adc_gpio_init(acs712_pin);
    _adc_channel = acs712_pin - 26u;
}
