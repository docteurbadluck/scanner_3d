#include "TestHardware_UC.hpp"

HardwareTestResult TestHardware_UC::execute()
{
    HardwareTestResult res;
    res.stepper = _stepper.selfTest();
    _sleep_ms(BETWEEN_TESTS_DELAY_MS);
    res.dc = _dc.selfTest();
    _sleep_ms(BETWEEN_TESTS_DELAY_MS);
    res.servo = _servo.selfTest();
    return res;
}
