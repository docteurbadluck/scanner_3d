#include "../Computer_Driver.hpp"
#include <unistd.h>

bool Computer_Driver::_sendWsHandshake()
{
	const std::string req =
		"GET / HTTP/1.1\r\nHost: " + _cfg.host + "\r\n"
		"Upgrade: websocket\r\nConnection: Upgrade\r\n"
		"Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
		"Sec-WebSocket-Version: 13\r\n\r\n";
	return write(_fd, req.c_str(), req.size()) == static_cast<ssize_t>(req.size());
}
