#include "../Computer_Driver.hpp"
#include <unistd.h>

void Computer_Driver::_writeFrameBytes(const std::vector<uint8_t> &frame)
{
    if (write(_fd, frame.data(), frame.size()) < 0)
    {
        close(_fd);
        _fd = -1;
    }
}
