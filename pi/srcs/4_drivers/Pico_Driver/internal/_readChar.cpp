#include "../Pico_Driver.hpp"
#include <sys/select.h>
#include <unistd.h>

bool Pico_Driver::_readChar(char &ch)
{
	fd_set         fds;
	struct timeval tv { _cfg.read_timeout_ms / 1000,
	                    (_cfg.read_timeout_ms % 1000) * 1000L };
	FD_ZERO(&fds);
	FD_SET(_fd, &fds);
	if (select(_fd + 1, &fds, nullptr, nullptr, &tv) <= 0)
		return false;
	return read(_fd, &ch, 1) == 1;
}
