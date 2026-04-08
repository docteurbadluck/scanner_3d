#include "Pico_Driver.hpp"
#include <unistd.h>

Pico_Driver::Pico_Driver(const Pico_DriverConfig &cfg)
	: _cfg(cfg), _fd(-1)
{
	_openPort();
}

Pico_Driver::~Pico_Driver()
{
	if (_fd >= 0)
		close(_fd);
}
