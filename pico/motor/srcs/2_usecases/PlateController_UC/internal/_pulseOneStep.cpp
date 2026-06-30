#include "../PlateController_UC.hpp"

bool PlateController_UC::_pulseOneStep()
{
    for (int p = 0; p < PULSES_PER_STEP; p++)
    {
        if (!_stepper.stepForward())
            return false;
    }
    return true;
}
