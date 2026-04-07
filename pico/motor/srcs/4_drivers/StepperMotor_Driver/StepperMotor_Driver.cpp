#include "StepperMotor_Driver.hpp"

StepperMotor_Driver::StepperMotor_Driver(const StepperMotor_DriverConfig &cfg,
                                         const StepperMotor_DriverPins   &pins)
    : _cfg(cfg), _pins(pins)
{
    _initGPIO();
}
{
