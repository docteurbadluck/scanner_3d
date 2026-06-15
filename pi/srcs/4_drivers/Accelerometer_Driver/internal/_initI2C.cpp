#include "../Accelerometer_Driver.hpp"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdexcept>

void Accelerometer_Driver::_initI2C()
{
    _fd = open(_cfg.device.c_str(), O_RDWR);
    if (_fd < 0)
        throw std::runtime_error("Cannot open I2C device: " + _cfg.device);
    if (ioctl(_fd, I2C_SLAVE, _cfg.address) < 0)
        throw std::runtime_error("Cannot set I2C slave address");
}
