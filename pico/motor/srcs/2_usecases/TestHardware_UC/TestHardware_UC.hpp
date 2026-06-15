#pragma once
#include "3_interface/IMotorDC.hpp"
#include "3_interface/IServoMotor.hpp"
#include "3_interface/IStepperMotor.hpp"

struct HardwareTestResult
{
    bool dc_ok;
    bool servo_ok;
    bool stepper_ok;
};

class TestHardware_UC
{
private:
    IMotorDC      &_dc;
    IServoMotor   &_servo;
    IStepperMotor &_stepper;

public:
    TestHardware_UC(IMotorDC &dc, IServoMotor &servo, IStepperMotor &stepper);
    ~TestHardware_UC() = default;

    [[nodiscard]] HardwareTestResult execute();
};
