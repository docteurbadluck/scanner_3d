#include "Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

bool Pico_Driver::isReady()
{
	return sendCommand(JsonMessage::makeCommand("PING"));
}
