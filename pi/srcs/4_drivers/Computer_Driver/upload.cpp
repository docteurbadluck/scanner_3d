#include "Computer_Driver.hpp"
#include <unistd.h>

bool Computer_Driver::upload(const std::string &data)
{
	const int fd = _tcpConnect(_cfg.http_port);
	if (fd < 0)
		return false;
	const bool ok = _sendHttpPost(fd, data);
	close(fd);
	return ok;
}
