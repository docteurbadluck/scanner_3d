#include "HandController_UC.hpp"

bool HandController_UC::joinInitialPos()
{
    if (_servo.goInitialPos())
    {
        _current_pos = Pos_hand::UP_A;
        return true;
    }
    _current_pos = Pos_hand::UNKNOWN;
    return false;
}
