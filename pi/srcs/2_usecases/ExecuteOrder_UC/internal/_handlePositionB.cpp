#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_handlePositionB()
{
    return _pico.goToPosition("POSITION_B") ? 0 : 1;
}
