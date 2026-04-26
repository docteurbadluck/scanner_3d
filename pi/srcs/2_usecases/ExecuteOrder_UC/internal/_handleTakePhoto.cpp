#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_handleTakePhoto()
{
    return (_capture.execute() && _send.execute()) ? 0 : 1;
}
