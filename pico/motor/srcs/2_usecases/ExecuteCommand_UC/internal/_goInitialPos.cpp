#include "../ExecuteCommand_UC.hpp"

int ExecuteCommand_UC::_goInitialPos()
{
    bool ok = _hand.joinPos(Pos_hand::SAFE) && _arm.joinInitialPos() && _hand.joinInitialPos();
    ok = _plate.joinInitialPos() && ok;
    return ok ? 0 : 1;
}
