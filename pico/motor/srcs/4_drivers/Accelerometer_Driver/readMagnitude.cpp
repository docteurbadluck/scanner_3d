#include "Accelerometer_Driver.hpp"

#include <cmath>

float Accelerometer_Driver::readMagnitude()
{
    int16_t x, y, z;
    _readRawAccel(x, y, z);
    const float mag = _toMagnitude(x, y, z);
    return fabsf(mag - 1.0f);
}
