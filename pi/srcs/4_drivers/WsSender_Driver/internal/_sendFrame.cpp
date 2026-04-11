#include "../WsSender_Driver.hpp"
#include <cstdint>
#include <vector>

void WsSender_Driver::_sendFrame(const std::string &payload)
{
	const uint8_t mask[4] = {0x12, 0x34, 0x56, 0x78};
	const size_t  len     = payload.size();
	std::vector<uint8_t> frame;
	frame.push_back(0x81u);
	_appendLength(frame, len, mask);
	for (size_t i = 0; i < len; ++i)
		frame.push_back(static_cast<uint8_t>(payload[i]) ^ mask[i % 4]);
	_writeFrame(frame);
}
