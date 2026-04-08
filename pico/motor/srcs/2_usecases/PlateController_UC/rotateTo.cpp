#include "PlateController_UC.hpp"

bool PlateController_UC::rotateTo(int target_step)
{
    if (_current_step < 0)
        return false;
    if (_current_step == target_step)
        return true;
    int steps = (target_step - _current_step + STEPS_PER_REVOLUTION) % STEPS_PER_REVOLUTION;
    return _rotateSteps(steps);
}
