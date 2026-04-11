#include "../WsSender_Driver.hpp"
#include <unistd.h>

bool WsSender_Driver::_readWsHandshakeOk()
{
	char        buf[256];
	std::string resp;
	ssize_t     n;
	while ((n = read(_fd, buf, sizeof(buf) - 1)) > 0)
	{
		buf[n] = '\0';
		resp  += buf;
		if (resp.find("\r\n\r\n") != std::string::npos)
			break;
	}
	return resp.find("101") != std::string::npos;
}
