#include "../Pico_Driver.hpp"

std::string Pico_Driver::_pollHardwareTest()
{
	const int saved      = _cfg.read_timeout_ms;
	_cfg.read_timeout_ms = 30000;
	std::string response;
	do {
		response = _readResponse();
	} while (!response.empty() && _isIntermediateState(response));
	_cfg.read_timeout_ms = saved;
	return response;
}
