#include "HandController_UC.hpp"

bool HandController_UC::joinPos(Pos_hand pos)
{
    if (_current_pos == pos)
        return true;
    if (_servo.goTo(pos))
    {
        _current_pos = pos;
        return true;
    }
    _current_pos = Pos_hand::UNKNOWN;
    return false;
}
