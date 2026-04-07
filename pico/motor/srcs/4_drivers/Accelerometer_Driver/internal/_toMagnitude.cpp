#include "Accelerometer_Driver.hpp"

#include <cmath>

namespace
{
static constexpr float ACCEL_SCALE = 16384.0f;
}

float Accelerometer_Driver::_toMagnitude(int16_t x, int16_t y, int16_t z) const
{
    const float fx = static_cast<float>(x) / ACCEL_SCALE;
    const float fy = static_cast<float>(y) / ACCEL_SCALE;
    const float fz = static_cast<float>(z) / ACCEL_SCALE;
    return sqrtf(fx * fx + fy * fy + fz * fz);
}
