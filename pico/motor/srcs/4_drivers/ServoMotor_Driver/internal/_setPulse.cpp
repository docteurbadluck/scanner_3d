#include "ServoMotor_Driver.hpp"

void ServoMotor_Driver::_setPulse(uint16_t pulse_us)
{
    pwm_set_chan_level(_pwm_slice, _pwm_channel, pulse_us);
}
