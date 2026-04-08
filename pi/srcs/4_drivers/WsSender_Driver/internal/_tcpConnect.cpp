#include "../WsSender_Driver.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int WsSender_Driver::_tcpConnect()
{
	const int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		return -1;
	struct hostent    *h = gethostbyname(_cfg.host.c_str());
	struct sockaddr_in addr{};
	if (!h) { close(fd); return -1; }
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(static_cast<uint16_t>(_cfg.ws_port));
	std::memcpy(&addr.sin_addr, h->h_addr, static_cast<size_t>(h->h_length));
	if (connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
	{ close(fd); return -1; }
	return fd;
}
