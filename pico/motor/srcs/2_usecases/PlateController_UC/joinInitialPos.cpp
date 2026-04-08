#include "PlateController_UC.hpp"

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
