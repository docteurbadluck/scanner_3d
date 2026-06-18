#include "Accelerometer_Driver.hpp"
#include <cmath>

float Accelerometer_Driver::readMagnitude()
{
    if (_fd < 0)
        return 0.0f;
    int16_t x, y, z;
    _readRawAccel(x, y, z);
    return fabsf(_toMagnitude(x, y, z) - 1.0f);
}
