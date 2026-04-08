#include "../ExecuteCommand_UC.hpp"

int ExecuteCommand_UC::_moveCamTo(Pos_hand pos)
{
    return _hand.joinPos(pos) ? 0 : 1;
}
