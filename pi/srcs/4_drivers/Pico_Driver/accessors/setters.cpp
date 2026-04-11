#include "../Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

bool Pico_Driver::setArmPosition(const std::string &pose)
{
	return sendCommand(JsonMessage::makeCommand(pose));
}

bool Pico_Driver::setCameraPosition(const std::string &pose)
{
	return sendCommand(JsonMessage::makeCommand(pose));
}
