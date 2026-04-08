#include "Computer_Driver.hpp"
#include <unistd.h>

std::string Computer_Driver::waitForOrder()
{
	if (_fd < 0 && !_connect())
		return "";
	const std::string frame = _readFrame();
	if (frame.empty())
	{
		close(_fd);
		_fd = -1;
	}
	return _extractCommand(frame);
}
