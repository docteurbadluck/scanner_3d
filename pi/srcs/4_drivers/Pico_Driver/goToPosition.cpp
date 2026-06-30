#include "Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

bool Pico_Driver::goToPosition(const std::string &cmd)
{
	const int saved      = _cfg.read_timeout_ms;
	_cfg.read_timeout_ms = 8000;
	const bool ok        = sendCommand(JsonMessage::makeCommand(cmd));
	_cfg.read_timeout_ms = saved;
	return ok;
}
