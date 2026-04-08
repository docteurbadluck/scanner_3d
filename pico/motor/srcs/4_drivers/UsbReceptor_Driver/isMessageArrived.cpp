#include "UsbReceptor_Driver.hpp"

#include <pico/stdlib.h>

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
