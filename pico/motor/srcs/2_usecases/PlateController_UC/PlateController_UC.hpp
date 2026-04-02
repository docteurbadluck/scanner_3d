#pragma once
#include "3_interface/IStepperMotor.hpp"

class PlateController_UC
{
private:
    IStepperMotor &_stepper;
    int            _current_step;

    static constexpr int STEPS_PER_REVOLUTION = 36;

    bool _rotateSteps(int steps);

public:
    PlateController_UC(IStepperMotor &stepper);
    ~PlateController_UC() = default;

    bool joinInitialPos();
    bool rotateTo(int target_step);
    int  getPos() const;
};
