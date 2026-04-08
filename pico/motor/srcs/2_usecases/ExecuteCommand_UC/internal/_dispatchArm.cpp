#include "../ExecuteCommand_UC.hpp"

int ExecuteCommand_UC::_dispatchArm(const std::string &cmd)
{
    if (cmd == "ARM_UP")   return _moveArmTo(Pos::UP);
    if (cmd == "ARM_DOWN") return _moveArmTo(Pos::DOWN);
    return -1;
}
