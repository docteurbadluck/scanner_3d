#include "../ExecuteOrder_UC.hpp"

int ExecuteOrder_UC::_handleStartCapture()
{
    return (_capture.execute() && _send.execute()) ? 0 : 1;
}
