#include "../ExecuteCommand_UC.hpp"

int ExecuteCommand_UC::_dispatch(const std::string &cmd)
{
    const int armRes = _dispatchArm(cmd);
    if (armRes != -1)
        return armRes;
    const int handRes = _dispatchHand(cmd);
    if (handRes != -1)
        return handRes;
    return _dispatchSystem(cmd);
}
