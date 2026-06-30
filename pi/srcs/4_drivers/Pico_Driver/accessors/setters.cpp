#include "../Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

bool Pico_Driver::setArmPosition(const std::string &pose)
{
	const int saved      = _cfg.read_timeout_ms;
	_cfg.read_timeout_ms = 8000;
	const bool ok        = sendCommand(JsonMessage::makeCommand(pose));
	_cfg.read_timeout_ms = saved;
	return ok;
}

bool Pico_Driver::setCameraPosition(const std::string &pose)
{
	return sendCommand(JsonMessage::makeCommand(pose));
}
