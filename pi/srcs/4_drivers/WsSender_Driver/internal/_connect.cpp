#include "../WsSender_Driver.hpp"
#include <unistd.h>

bool WsSender_Driver::_connect()
{
	_fd = _tcpConnect();
	if (_fd < 0)
		return false;
	if (!_sendWsHandshake() || !_readWsHandshakeOk())
	{
		close(_fd);
		_fd = -1;
		return false;
	}
	return true;
}
