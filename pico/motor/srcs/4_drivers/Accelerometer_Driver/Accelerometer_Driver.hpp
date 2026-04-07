#pragma once

#include "3_interface/IAccelerometer.hpp"

#include <hardware/i2c.h>
#include <cstdint>

struct Accelerometer_DriverConfig
{
    i2c_inst_t *i2c_port  = i2c0;
    uint8_t     address   = 0x68;
    uint32_t    baud_rate = 400000;
};

struct Accelerometer_DriverPins
{
    uint8_t sda_pin = 4;
    uint8_t scl_pin = 5;
};

class Accelerometer_Driver : public IAccelerometer
{
public:
    Accelerometer_Driver(const Accelerometer_DriverConfig &cfg,
                         const Accelerometer_DriverPins   &pins);
    ~Accelerometer_Driver() override = default;

    float readMagnitude() override;

private:
    Accelerometer_DriverConfig _cfg;
    Accelerometer_DriverPins   _pins;

    void  _initI2C();
    void  _wakeUp();
    void  _readRawAccel(int16_t &x, int16_t &y, int16_t &z) const;
    float _toMagnitude(int16_t x, int16_t y, int16_t z) const;
};
