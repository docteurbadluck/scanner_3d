#include "../Computer_Driver.hpp"
#include <cstdint>

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
