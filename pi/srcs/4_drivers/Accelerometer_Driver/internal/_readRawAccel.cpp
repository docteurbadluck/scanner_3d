#include "../Accelerometer_Driver.hpp"
#include <unistd.h>

bool Accelerometer_Driver::_readRawAccel(int16_t &x, int16_t &y, int16_t &z) const
{
    static constexpr uint8_t REG_ACCEL_XOUT = 0x3B;
    uint8_t raw[6];
    if (write(_fd, &REG_ACCEL_XOUT, 1) != 1)
        return false;
    if (read(_fd, raw, 6) != 6)
        return false;
    x = static_cast<int16_t>((raw[0] << 8) | raw[1]);
    y = static_cast<int16_t>((raw[2] << 8) | raw[3]);
    z = static_cast<int16_t>((raw[4] << 8) | raw[5]);
    return true;
}
