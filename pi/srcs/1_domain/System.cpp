#include "System.hpp"

System::System() : _current_state(INITIALIZATION)
{
}

state System::get_state() const
{
	return _current_state;
}

Commands System::getCommands() const
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

void System::setCommandToExecute(const std::string &command)
{
	_command_to_execute = command;
}

std::string System::getCommandToExecute() const
{
	return _command_to_execute;
}

void System::ready()
{
	_current_state = READY;
}

void System::processing()
{
	_current_state = PROCESSING;
}

void System::sending()
{
	_current_state = SENDING;
}

void System::reset()
{
	_current_state = INITIALIZATION;
}
