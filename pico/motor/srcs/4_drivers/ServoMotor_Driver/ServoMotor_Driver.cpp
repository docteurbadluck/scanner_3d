#include "ServoMotor_Driver.hpp"

#include <hardware/gpio.h>

ServoMotor_Driver::ServoMotor_Driver(const ServoMotor_DriverConfig &cfg,
                                     const ServoMotor_DriverPins   &pins)
    : _cfg(cfg), _pins(pins), _pwm_slice(0), _pwm_channel(0)
{
    _initPWM();
}
