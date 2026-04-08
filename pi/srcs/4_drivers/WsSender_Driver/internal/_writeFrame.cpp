#include "../WsSender_Driver.hpp"
#include <cstdint>
#include <unistd.h>
#include <vector>

void WsSender_Driver::_writeFrame(const std::vector<uint8_t> &frame)
{
	if (write(_fd, frame.data(), frame.size()) < 0)
	{
		close(_fd);
		_fd = -1;
	}
}
