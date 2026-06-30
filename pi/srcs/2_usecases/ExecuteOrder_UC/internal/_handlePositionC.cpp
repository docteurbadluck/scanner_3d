#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_handlePositionC()
{
    return _pico.goToPosition("POSITION_C") ? 0 : 1;
}
