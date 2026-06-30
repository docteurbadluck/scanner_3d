#include "../ExecuteCommand_UC.hpp"

int ExecuteCommand_UC::_moveToPosition(Pos armPos, Pos_hand camPos)
{
    bool ok;
    if (_arm.getPos() != armPos)
        ok = _hand.joinPos(Pos_hand::SAFE) && _arm.joinPos(armPos) && _hand.joinPos(camPos);
    else
        ok = _arm.joinPos(armPos) && _hand.joinPos(camPos);
    return ok ? 0 : 1;
}
