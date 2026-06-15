#include "TestHardware_UC.hpp"

HardwareTestResult TestHardware_UC::execute()
{
    return {
        _dc.selfTest()      == SelfTestResult::OK,
        _servo.selfTest()   == ServoSelfTestResult::OK,
        _stepper.selfTest() == StepperSelfTestResult::OK,
    };
}
