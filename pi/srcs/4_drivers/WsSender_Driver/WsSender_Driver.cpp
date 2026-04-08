#include "WsSender_Driver.hpp"
#include <unistd.h>

WsSender_Driver::WsSender_Driver(const WsSender_DriverConfig &cfg)
	: _cfg(cfg), _fd(-1)
{
}

WsSender_Driver::~WsSender_Driver()
{
	if (_fd >= 0)
		close(_fd);
}
