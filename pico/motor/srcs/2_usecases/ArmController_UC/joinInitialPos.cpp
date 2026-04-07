#include "ArmController_UC.hpp"

bool ArmController_UC::joinInitialPos()
{
    if (_motor.goInitialPos())
    {
        _current_pos = Pos::UP;
        return true;
    }
    _current_pos = Pos::UNKNOWN;
    return false;
}
