#include "UsbReceptor_Driver.hpp"

std::string UsbReceptor_Driver::getMessage() const
{
    std::string msg = _buffer;
    _buffer.clear();
    return msg;
}
