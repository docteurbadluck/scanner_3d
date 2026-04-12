#include "../Computer_Driver.hpp"
#include <cstdint>

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
