#include "../Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

bool Pico_Driver::_isSuccess(const std::string &response)
{
	return JsonMessage::extractStringField(response, "kind") == "ok";
}
