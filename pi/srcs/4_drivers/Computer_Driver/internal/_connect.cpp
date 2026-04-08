#include "../Computer_Driver.hpp"
#include <unistd.h>

bool Computer_Driver::_connect()
{
	_fd = _tcpConnect(_cfg.ws_port);
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
