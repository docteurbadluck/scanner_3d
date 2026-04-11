#include "../Computer_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

std::string Computer_Driver::_extractCommand(const std::string &json)
{
	return JsonMessage::extractStringField(json, "command");
}
