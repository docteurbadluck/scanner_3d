#include "../Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

bool Pico_Driver::_isSuccess(const std::string &response)
{
	const std::string kind = JsonMessage::extractStringField(response, "kind");
	return kind == "DONE" || kind == "PONG";
}
