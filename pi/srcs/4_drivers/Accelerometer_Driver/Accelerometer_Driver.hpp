#pragma once
#include "3_interface/IAccelerometer.hpp"
#include <cstdint>
#include <string>

struct Accelerometer_DriverConfig
{
    std::string device  = "/dev/i2c-1";
    uint8_t     address = 0x68;
};

class Accelerometer_Driver : public IAccelerometer
{
public:
    explicit Accelerometer_Driver(const Accelerometer_DriverConfig &cfg);
    ~Accelerometer_Driver() override;

    float readMagnitude() override;
    void  readGyro(float &gx, float &gy, float &gz);

private:
    Accelerometer_DriverConfig _cfg;
    int                        _fd;

    void    _initI2C();
    void    _wakeUp();
    void    _readRawAccel(int16_t &x, int16_t &y, int16_t &z) const;
    void    _readRawGyro(int16_t &x, int16_t &y, int16_t &z) const;
    float   _toMagnitude(int16_t x, int16_t y, int16_t z) const;
};
