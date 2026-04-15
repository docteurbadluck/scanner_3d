#include "../Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

std::string Pico_Driver::getPicoStatus()
{
	if (!_writeLine(JsonMessage::makeCommand("GET_STATUS")))
		return JsonMessage::makeError("write_failed");
	std::string response;
	do {
		response = _readResponse();
	} while (JsonMessage::extractStringField(response, "state") == "INTERPRETING"
	      || JsonMessage::extractStringField(response, "state") == "EXECUTING");
	return response;
}
