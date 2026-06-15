#pragma once

enum class StepperSelfTestResult { OK };

class IStepperMotor
{
public:
    virtual ~IStepperMotor() = default;
    virtual bool                 goInitialPos() = 0;
    virtual bool                 stepForward()  = 0;
    virtual StepperSelfTestResult selfTest()     = 0;
};
