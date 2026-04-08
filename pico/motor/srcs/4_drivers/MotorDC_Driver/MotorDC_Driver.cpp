#include "MotorDC_Driver.hpp"

MotorDC_Driver::MotorDC_Driver(const MotorDC_DriverConfig &cfg, MotorDC_DriverIO io)
    : _cfg(cfg), _io(std::move(io))
{
}

