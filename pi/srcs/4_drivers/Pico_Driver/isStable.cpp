#include "Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

bool Pico_Driver::isStable()
{
	if (!_writeLine(JsonMessage::makeCommand("GET_STATUS")))
		return false;
	const std::string response = _readResponse();
	const std::string state    = JsonMessage::extractStringField(response, "state");
	return state == "LISTENING";
}
