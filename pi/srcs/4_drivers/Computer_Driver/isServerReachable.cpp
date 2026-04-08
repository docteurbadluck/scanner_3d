#include "Computer_Driver.hpp"
#include <unistd.h>

bool Computer_Driver::isServerReachable()
{
	const int fd = _tcpConnect(_cfg.http_port);
	if (fd < 0)
		return false;
	close(fd);
	return true;
}
