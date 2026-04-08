#include "Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

bool Pico_Driver::rotatePlateStep()
{
	return sendCommand(JsonMessage::makeCommand("PLATE_NEXT"));
}
