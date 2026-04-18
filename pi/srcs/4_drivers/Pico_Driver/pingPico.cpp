#include "Pico_Driver.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"
#include <chrono>

std::string Pico_Driver::pingPico()
{
	const auto start = std::chrono::steady_clock::now();
	if (!_writeLine(JsonMessage::makeCommand("PING")))
		return JsonMessage::makeError("write_failed");
	_readResponse();
	const long ms = std::chrono::duration_cast<std::chrono::milliseconds>(
	    std::chrono::steady_clock::now() - start).count();
	return JsonMessage::makePingPicoResponse(ms);
}
