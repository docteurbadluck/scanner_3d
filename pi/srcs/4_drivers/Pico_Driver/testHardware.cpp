#include "Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

std::string Pico_Driver::testHardware()
{
	if (!_writeLine(JsonMessage::makeCommand("TEST_HARDWARE")))
		return JsonMessage::makeError("write_failed");
	std::string response;
	do {
		response = _readResponse();
	} while (!response.empty()
	      && (JsonMessage::extractStringField(response, "state") == "INTERPRETING"
	       || JsonMessage::extractStringField(response, "state") == "EXECUTING"));
	return response;
}
