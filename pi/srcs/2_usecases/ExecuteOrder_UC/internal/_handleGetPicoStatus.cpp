#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_handleGetPicoStatus()
{
    _sender.sendPicoStatus(_pico.getPicoStatus());
    return 0;
}
