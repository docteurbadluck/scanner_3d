#include "Accelerometer_Driver.hpp"
#include <unistd.h>

Accelerometer_Driver::Accelerometer_Driver(const Accelerometer_DriverConfig &cfg)
    : _cfg(cfg), _fd(-1)
{
    _initI2C();
    _wakeUp();
}

Accelerometer_Driver::~Accelerometer_Driver()
{
    if (_fd >= 0)
        close(_fd);
}
