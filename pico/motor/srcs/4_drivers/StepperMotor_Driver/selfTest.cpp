#include "StepperMotor_Driver.hpp"

static constexpr uint16_t N_TEST_STEPS = 400;

StepperSelfTestResult StepperMotor_Driver::selfTest()
{
    _enable();
    for (uint16_t i = 0; i < N_TEST_STEPS; ++i)
        stepForward();
    for (uint16_t i = 0; i < N_TEST_STEPS; ++i)
        _stepBackward();
    return StepperSelfTestResult::OK;
}
