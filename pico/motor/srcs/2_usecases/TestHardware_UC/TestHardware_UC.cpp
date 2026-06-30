#include "TestHardware_UC.hpp"

TestHardware_UC::TestHardware_UC(IMotorDC &dc, IServoMotor &servo, IStepperMotor &stepper,
    std::function<void(uint32_t)> sleep_ms)
    : _dc(dc), _servo(servo), _stepper(stepper), _sleep_ms(sleep_ms)
{}

