#include "../Accelerometer_Driver.hpp"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>

void Accelerometer_Driver::_initI2C()
{
    _fd = open(_cfg.device.c_str(), O_RDWR);
    if (_fd < 0)
        return;
    if (ioctl(_fd, I2C_SLAVE, _cfg.address) < 0)
    {
        close(_fd);
        _fd = -1;
    }
}
