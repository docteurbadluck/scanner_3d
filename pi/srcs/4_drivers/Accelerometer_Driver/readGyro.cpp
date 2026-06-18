#include "Accelerometer_Driver.hpp"

void Accelerometer_Driver::readGyro(float &gx, float &gy, float &gz)
{
    static constexpr float GYRO_SCALE = 131.0f;
    if (_fd < 0)
    {
        gx = gy = gz = 0.0f;
        return;
    }
    int16_t x, y, z;
    _readRawGyro(x, y, z);
    gx = static_cast<float>(x) / GYRO_SCALE;
    gy = static_cast<float>(y) / GYRO_SCALE;
    gz = static_cast<float>(z) / GYRO_SCALE;
}
