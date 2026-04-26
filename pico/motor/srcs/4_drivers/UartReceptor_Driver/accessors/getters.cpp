#include "UartReceptor_Driver.hpp"

std::string UartReceptor_Driver::getMessage() const
{
    std::string msg = _buffer;
    _buffer.clear();
    return msg;
}
