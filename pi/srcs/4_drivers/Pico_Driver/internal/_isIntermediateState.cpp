#include "../Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

bool Pico_Driver::_isIntermediateState(const std::string &response)
{
	const auto state = JsonMessage::extractStringField(response, "state");
	return state == "INTERPRETING" || state == "EXECUTING";
}
