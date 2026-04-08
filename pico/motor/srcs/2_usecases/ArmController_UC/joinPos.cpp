#include "ArmController_UC.hpp"

bool ArmController_UC::joinPos(Pos pos)
{
    if (_current_pos == pos)
        return true;
    if (_motor.goTo(pos))
    {
        _current_pos = pos;
        return true;
    }
    _current_pos = Pos::UNKNOWN;
    return false;
}
