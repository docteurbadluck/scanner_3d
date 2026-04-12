#include "Computer_Driver.hpp"

void Computer_Driver::send(const std::string &message)
{
    if (_fd < 0 && !_connect())
        return;
    _sendFrame(message);
}
