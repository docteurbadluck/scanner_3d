#include "../ExecuteCommand_UC.hpp"

int ExecuteCommand_UC::_goInitialPos()
{
    bool ok = _arm.joinInitialPos();
    ok = _hand.joinInitialPos() && ok;
    ok = _plate.joinInitialPos() && ok;
    return ok ? 0 : 1;
}
