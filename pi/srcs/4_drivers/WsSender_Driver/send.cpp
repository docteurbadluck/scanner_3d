#include "WsSender_Driver.hpp"

void WsSender_Driver::send(const std::string &message)
{
	if (_fd < 0 && !_connect())
		return;
	_sendFrame(message);
}
