#include "../ExecuteCommand_UC.hpp"

int ExecuteCommand_UC::_moveArmTo(Pos pos)
{
    return _arm.joinPos(pos) ? 0 : 1;
}
