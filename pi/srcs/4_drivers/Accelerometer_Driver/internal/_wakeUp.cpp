#include "../Accelerometer_Driver.hpp"
#include <unistd.h>

void Accelerometer_Driver::_wakeUp()
{
    static constexpr uint8_t REG_PWR_MGMT_1 = 0x6B;
    uint8_t buf[2] = { REG_PWR_MGMT_1, 0x00 };
    write(_fd, buf, 2);
}
