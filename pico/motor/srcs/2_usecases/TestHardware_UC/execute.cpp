#include "TestHardware_UC.hpp"

HardwareTestResult TestHardware_UC::execute()
{
    return {
        _dc.selfTest(),
        _servo.selfTest(),
        _stepper.selfTest(),
    };
}
