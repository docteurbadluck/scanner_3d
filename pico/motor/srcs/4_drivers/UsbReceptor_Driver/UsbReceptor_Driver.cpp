#include "UsbReceptor_Driver.hpp"

#include <pico/stdlib.h>

UsbReceptor_Driver::UsbReceptor_Driver(const UsbReceptor_DriverConfig &cfg)
    : _cfg(cfg)
{
}

bool UsbReceptor_Driver::isMessageArrived()
{
    const int c = getchar_timeout_us(_cfg.poll_timeout_us);

    if (c < 0)
        return false;

    if (_buffer.size() >= _cfg.max_msg_size)
        _buffer.clear();

    const char ch = static_cast<char>(c);
    _buffer += ch;
    return ch == _cfg.delimiter;
}

std::string UsbReceptor_Driver::getMessage() const
{
    std::string msg = _buffer;
    _buffer.clear();
    return msg;
}
