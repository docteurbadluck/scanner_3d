#include "ServoMotor_Driver.hpp"

#include <hardware/adc.h>
#include <pico/stdlib.h>
#include <algorithm>

uint16_t ServoMotor_Driver::_readAdcPeak(uint32_t duration_ms) const
{
    uint16_t peak = 0;
    for (uint32_t elapsed = 0; elapsed < duration_ms; elapsed += 5)
    {
        adc_select_input(_adc_channel);
        peak = std::max(peak, adc_read());
        ::sleep_ms(5);
    }
    return peak;
}
