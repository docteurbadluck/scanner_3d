#include "Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

bool Pico_Driver::isReady()
{
	if (!_writeLine(JsonMessage::makeCommand("PING")))
		return false;
	return _isSuccess(_readResponse());
}
