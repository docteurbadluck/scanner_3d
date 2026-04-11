#include "Pico_Driver.hpp"

bool Pico_Driver::sendCommand(const std::string &cmd)
{
	if (!_writeLine(cmd))
		return false;
	const std::string response = _readResponse();
	return _isSuccess(response);
}
