#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_handlePingPico()
{
    _sender.sendRaw(_pico.pingPico());
    return 0;
}
