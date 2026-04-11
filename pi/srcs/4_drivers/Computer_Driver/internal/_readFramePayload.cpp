#include "../Computer_Driver.hpp"
#include <unistd.h>

std::string Computer_Driver::_readFramePayload(size_t len)
{
	if (len == 0)
		return "";
	std::string payload(len, '\0');
	if (read(_fd, &payload[0], len) != static_cast<ssize_t>(len))
		return "";
	return payload;
}
