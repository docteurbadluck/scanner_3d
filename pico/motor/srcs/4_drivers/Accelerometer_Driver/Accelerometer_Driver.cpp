#include "Accelerometer_Driver.hpp"

#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include <cmath>

static constexpr uint8_t  REG_PWR_MGMT_1  = 0x6B;
static constexpr uint8_t  REG_ACCEL_XOUT  = 0x3B;
static constexpr float    ACCEL_SCALE     = 16384.0f;

Accelerometer_Driver::Accelerometer_Driver(const Accelerometer_DriverConfig &cfg,
                                           const Accelerometer_DriverPins   &pins)
    : _cfg(cfg), _pins(pins)
{
    _initI2C();
    _wakeUp();
}

void Accelerometer_Driver::_initI2C()
{
    i2c_init(_cfg.i2c_port, _cfg.baud_rate);
    gpio_set_function(_pins.sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(_pins.scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(_pins.sda_pin);
    gpio_pull_up(_pins.scl_pin);
}

void Accelerometer_Driver::_wakeUp()
{
    uint8_t buf[2] = { REG_PWR_MGMT_1, 0x00 };
    i2c_write_blocking(_cfg.i2c_port, _cfg.address, buf, 2, false);
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

float Accelerometer_Driver::_toMagnitude(int16_t x, int16_t y, int16_t z) const
{
    const float fx = static_cast<float>(x) / ACCEL_SCALE;
    const float fy = static_cast<float>(y) / ACCEL_SCALE;
    const float fz = static_cast<float>(z) / ACCEL_SCALE;
    return sqrtf(fx * fx + fy * fy + fz * fz);
}

float Accelerometer_Driver::readMagnitude()
{
    int16_t x, y, z;
    _readRawAccel(x, y, z);
    const float mag = _toMagnitude(x, y, z);
    return fabsf(mag - 1.0f);
}
