#include "../System.hpp"

state System::get_state() const
{
	return _current_state;
}

const Commands& System::getCommands() const
{
	return _commands;
}

std::string System::getStateString() const
{
	if (_current_state == INITIALIZATION) return "INITIALIZATION";
	if (_current_state == READY)          return "READY";
	if (_current_state == PROCESSING)     return "PROCESSING";
	return "SENDING";
}

std::string System::getCommandToExecute() const
{
	return _command_to_execute;
}
