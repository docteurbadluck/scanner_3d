#include "../MotorDC_Driver.hpp"

#include <hardware/adc.h>

void MotorDC_Driver::_initADC(uint8_t adc_pin, uint8_t adc_pin_2)
{
    adc_init();
    adc_gpio_init(adc_pin);
    adc_gpio_init(adc_pin_2);
    _adc_channel   = adc_pin   - 26u;
    _adc_channel_2 = adc_pin_2 - 26u;
}
