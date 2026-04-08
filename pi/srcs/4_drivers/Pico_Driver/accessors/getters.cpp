#include "../Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

std::string Pico_Driver::getPicoStatus()
{
	if (!_writeLine(JsonMessage::makeCommand("GET_STATUS")))
		return JsonMessage::makeError("write_failed");
	return _readResponse();
}
