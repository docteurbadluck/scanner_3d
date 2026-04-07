#include "../PlateController_UC.hpp"

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
