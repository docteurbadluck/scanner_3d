#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_handleInitialPos()
{
    return _pico.goInitialPos() ? 0 : 1;
}
