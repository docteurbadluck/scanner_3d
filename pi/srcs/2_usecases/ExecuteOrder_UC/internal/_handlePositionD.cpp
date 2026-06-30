#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_handlePositionD()
{
    return _pico.goToPosition("POSITION_D") ? 0 : 1;
}
