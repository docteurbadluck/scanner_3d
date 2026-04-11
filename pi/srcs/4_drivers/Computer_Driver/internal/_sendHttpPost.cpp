#include "../Computer_Driver.hpp"
#include <unistd.h>

static bool write_all(int fd, const char *buf, size_t len)
{
	size_t sent = 0;
	while (sent < len)
	{
		ssize_t n = write(fd, buf + sent, len - sent);
		if (n <= 0)
			return false;
		sent += static_cast<size_t>(n);
	}
	return true;
}

bool Computer_Driver::_sendHttpPost(int fd, const std::string &data)
{
	const std::string header =
		"POST /upload HTTP/1.1\r\n"
		"Host: " + _cfg.host + "\r\n"
		"Content-Type: application/octet-stream\r\n"
		"Content-Length: " + std::to_string(data.size()) + "\r\n\r\n";
	if (!write_all(fd, header.c_str(), header.size()))
		return false;
	return write_all(fd, data.c_str(), data.size());
}
