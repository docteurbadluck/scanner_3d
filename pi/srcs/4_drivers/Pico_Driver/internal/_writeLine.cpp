#include "../Pico_Driver.hpp"
#include <unistd.h>

bool Pico_Driver::_writeLine(const std::string &line)
{
	if (_fd < 0)
		_openPort();
	if (_fd < 0)
		return false;
	const std::string msg     = line + "\n";
	const ssize_t     written = write(_fd, msg.c_str(), msg.size());
	return written == static_cast<ssize_t>(msg.size());
}
