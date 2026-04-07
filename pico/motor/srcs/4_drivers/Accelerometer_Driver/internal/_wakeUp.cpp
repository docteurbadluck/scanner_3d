#include "Accelerometer_Driver.hpp"

#include <hardware/i2c.h>

namespace
{
static constexpr uint8_t REG_PWR_MGMT_1 = 0x6B;
}

void Accelerometer_Driver::_wakeUp()
{
    uint8_t buf[2] = { REG_PWR_MGMT_1, 0x00 };
    i2c_write_blocking(_cfg.i2c_port, _cfg.address, buf, 2, false);
}
