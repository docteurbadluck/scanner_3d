#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_handlePlateNext()
{
    return _pico.rotatePlateStep() ? 0 : 1;
}
