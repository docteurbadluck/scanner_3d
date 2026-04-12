#include "Computer_Driver.hpp"
#include <cstdint>
#include <unistd.h>
#include <vector>

void Computer_Driver::_appendLen(
    std::vector<uint8_t> &frame, size_t len, const uint8_t mask[4])
{
    if (len < 126)
        frame.push_back(0x80u | static_cast<uint8_t>(len));
    else
    {
        frame.push_back(0x80u | 126u);
        frame.push_back(static_cast<uint8_t>((len >> 8) & 0xFFu));
        frame.push_back(static_cast<uint8_t>(len & 0xFFu));
    }
    frame.insert(frame.end(), mask, mask + 4);
}

void Computer_Driver::_writeFrameBytes(const std::vector<uint8_t> &frame)
{
    if (write(_fd, frame.data(), frame.size()) < 0)
    {
        close(_fd);
        _fd = -1;
    }
}

void Computer_Driver::_sendFrame(const std::string &payload)
{
    const uint8_t mask[4] = {0x12, 0x34, 0x56, 0x78};
    const size_t  len     = payload.size();
    std::vector<uint8_t> frame;
    frame.push_back(0x81u);
    _appendLen(frame, len, mask);
    for (size_t i = 0; i < len; ++i)
        frame.push_back(static_cast<uint8_t>(payload[i]) ^ mask[i % 4]);
    _writeFrameBytes(frame);
}

void Computer_Driver::send(const std::string &message)
{
    if (_fd < 0 && !_connect())
        return;
    _sendFrame(message);
}
