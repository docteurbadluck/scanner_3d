#include "Accelerometer_Driver.hpp"

#include <hardware/i2c.h>

namespace
{
static constexpr uint8_t REG_ACCEL_XOUT = 0x3B;
}

void Accelerometer_Driver::_readRawAccel(int16_t &x, int16_t &y, int16_t &z) const
{
    uint8_t reg = REG_ACCEL_XOUT;
    uint8_t raw[6];

    i2c_write_blocking(_cfg.i2c_port, _cfg.address, &reg, 1, true);
    i2c_read_blocking(_cfg.i2c_port, _cfg.address, raw, 6, false);

    x = static_cast<int16_t>((raw[0] << 8) | raw[1]);
    y = static_cast<int16_t>((raw[2] << 8) | raw[3]);
    z = static_cast<int16_t>((raw[4] << 8) | raw[5]);
}
