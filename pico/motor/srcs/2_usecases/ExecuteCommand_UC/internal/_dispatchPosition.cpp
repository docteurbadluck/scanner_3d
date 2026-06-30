#include "../ExecuteCommand_UC.hpp"

int ExecuteCommand_UC::_dispatchPosition(const std::string &cmd)
{
    if (cmd == "POSITION_A") return _moveToPosition(Pos::UP, Pos_hand::UP_A);
    if (cmd == "POSITION_B") return _moveToPosition(Pos::UP, Pos_hand::UP_B);
    if (cmd == "POSITION_C") return _moveToPosition(Pos::DOWN, Pos_hand::DOWN_A);
    if (cmd == "POSITION_D") return _moveToPosition(Pos::DOWN, Pos_hand::DOWN_B);
    return -1;
}
