#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_handlePositionA()
{
    return _pico.goToPosition("POSITION_A") ? 0 : 1;
}
