#include "PlateController_UC.hpp"

PlateController_UC::PlateController_UC(IStepperMotor &stepper)
    : _stepper(stepper), _current_step(-1)
{}

bool PlateController_UC::joinInitialPos()
{
    if (_stepper.goInitialPos())
    {
        _current_step = 0;
        return true;
    }
    _current_step = -1;
    return false;
}

bool PlateController_UC::_rotateSteps(int steps)
{
    for (int i = 0; i < steps; i++)
    {
        if (!_stepper.stepForward())
        {
            _current_step = -1;
            return false;
        }
        _current_step = (_current_step + 1) % STEPS_PER_REVOLUTION;
    }
    return true;
}

bool PlateController_UC::rotateTo(int target_step)
{
    if (_current_step < 0)
        return false;
    if (_current_step == target_step)
        return true;
    int steps = (target_step - _current_step + STEPS_PER_REVOLUTION) % STEPS_PER_REVOLUTION;
    return _rotateSteps(steps);
}

int PlateController_UC::getPos() const
{
    return _current_step;
}
