#include "Accelerometer_Driver.hpp"

Accelerometer_Driver::Accelerometer_Driver(const Accelerometer_DriverConfig &cfg,
                                           const Accelerometer_DriverPins   &pins)
    : _cfg(cfg), _pins(pins)
{
    _initI2C();
    _wakeUp();
}
