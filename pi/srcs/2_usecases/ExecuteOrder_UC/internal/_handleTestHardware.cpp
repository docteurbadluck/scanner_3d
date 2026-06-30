#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_handleTestHardware()
{
    _sender.sendRaw(_pico.testHardware());
    return 0;
}
