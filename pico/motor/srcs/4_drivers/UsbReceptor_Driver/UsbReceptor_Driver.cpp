#include "UsbReceptor_Driver.hpp"

#include <pico/stdlib.h>

UsbReceptor_Driver::UsbReceptor_Driver(const UsbReceptor_DriverConfig &cfg)
    : _cfg(cfg)
{
}

bool UsbReceptor_Driver::isMessageArrived()
{
    const int c = getchar_timeout_us(_cfg.poll_timeout_us);

    if (c == PICO_ERROR_TIMEOUT)
        return false;

    _buffer += static_cast<char>(c);
    return static_cast<char>(c) == _cfg.delimiter;
}

std::string UsbReceptor_Driver::getMessage() const
{
    std::string msg = _buffer;
    _buffer.clear();
    return msg;
}
