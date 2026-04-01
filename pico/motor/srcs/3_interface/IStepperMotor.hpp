#pragma once

class IStepperMotor
{
public:
    virtual ~IStepperMotor() = default;
    virtual bool goInitialPos() = 0;
    virtual bool stepForward() = 0;
};
