#include "../ExecuteCommand_UC.hpp"

int ExecuteCommand_UC::_dispatchHand(const std::string &cmd)
{
    if (cmd == "CAM_A") return _moveCamTo(Pos_hand::UP_A);
    if (cmd == "CAM_B") return _moveCamTo(Pos_hand::UP_B);
    if (cmd == "CAM_C") return _moveCamTo(Pos_hand::DOWN_A);
    if (cmd == "CAM_D") return _moveCamTo(Pos_hand::DOWN_B);
    return -1;
}
