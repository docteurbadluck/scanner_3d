#include "Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

std::string Pico_Driver::testHardware()
{
	if (!_writeLine(JsonMessage::makeCommand("TEST_HARDWARE")))
		return JsonMessage::makeError("write_failed");
	const std::string response = _pollHardwareTest();
	if (response.empty())
		return JsonMessage::makeError("hardware_test_timeout");
	return response;
}
