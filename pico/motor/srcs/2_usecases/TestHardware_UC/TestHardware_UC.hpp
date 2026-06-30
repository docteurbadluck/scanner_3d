#pragma once
#include "3_interface/IMotorDC.hpp"
#include "3_interface/IServoMotor.hpp"
#include "3_interface/IStepperMotor.hpp"

#include <cstdint>
#include <functional>

struct HardwareTestResult
{
    SelfTestResult        dc;
    ServoSelfTestResult   servo;
    StepperSelfTestResult stepper;
};

class TestHardware_UC
{
private:
    static constexpr uint32_t BETWEEN_TESTS_DELAY_MS = 500;

    IMotorDC      &_dc;
    IServoMotor   &_servo;
    IStepperMotor &_stepper;
    std::function<void(uint32_t)> _sleep_ms;

public:
    TestHardware_UC(IMotorDC &dc, IServoMotor &servo, IStepperMotor &stepper,
        std::function<void(uint32_t)> sleep_ms);
    ~TestHardware_UC() = default;

    [[nodiscard]] HardwareTestResult execute();
};
