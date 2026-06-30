#include "../Accelerometer_Driver.hpp"
#include <unistd.h>

void Accelerometer_Driver::_readRawGyro(int16_t &x, int16_t &y, int16_t &z) const
{
    static constexpr uint8_t REG_GYRO_XOUT = 0x43;
    uint8_t raw[6];
    write(_fd, &REG_GYRO_XOUT, 1);
    read(_fd, raw, 6);
    x = static_cast<int16_t>((raw[0] << 8) | raw[1]);
    y = static_cast<int16_t>((raw[2] << 8) | raw[3]);
    z = static_cast<int16_t>((raw[4] << 8) | raw[5]);
}
