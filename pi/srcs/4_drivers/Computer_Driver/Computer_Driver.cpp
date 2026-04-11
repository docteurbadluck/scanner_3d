#include "Computer_Driver.hpp"
#include <unistd.h>

Computer_Driver::Computer_Driver(const Computer_DriverConfig &cfg)
	: _cfg(cfg), _fd(-1)
{
}

Computer_Driver::~Computer_Driver()
{
	if (_fd >= 0)
		close(_fd);
}
