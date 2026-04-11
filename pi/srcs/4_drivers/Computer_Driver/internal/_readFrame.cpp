#include "../Computer_Driver.hpp"
#include <cstdint>
#include <unistd.h>

std::string Computer_Driver::_readFrame()
{
	uint8_t header[2];
	if (read(_fd, header, 2) != 2)
		return "";
	size_t len = header[1] & 0x7Fu;
	if (len == 126)
	{
		uint8_t ext[2];
		if (read(_fd, ext, 2) != 2)
			return "";
		len = (static_cast<size_t>(ext[0]) << 8) | ext[1];
	}
	return _readFramePayload(len);
}
