#include "../Computer_Driver.hpp"
#include <unistd.h>

bool Computer_Driver::_sendHttpPost(int fd, const std::string &data)
{
	const std::string header =
		"POST /upload HTTP/1.1\r\n"
		"Host: " + _cfg.host + "\r\n"
		"Content-Type: application/octet-stream\r\n"
		"Content-Length: " + std::to_string(data.size()) + "\r\n\r\n";
	if (write(fd, header.c_str(), header.size()) < 0)
		return false;
	return write(fd, data.c_str(), data.size()) == static_cast<ssize_t>(data.size());
}
