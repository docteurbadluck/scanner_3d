#include "Pico_Driver.hpp"

bool Pico_Driver::sendCommand(const std::string &cmd)
{
	if (!_writeLine(cmd))
		return false;
	return _isSuccess(_readResponse());
}
