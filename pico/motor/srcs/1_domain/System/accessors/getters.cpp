#include "1_domain/System/System.hpp"

state System::getState() const
{
    return _current_state;
}

std::string System::getStateString() const
{
    if (_current_state == LISTENING)    return "LISTENING";
    if (_current_state == INTERPRETING) return "INTERPRETING";
    return "EXECUTING";
}

std::string System::getCommandToExecute() const
{
    return _command_to_execute;
}

Commands System::getCommands() const
{
    return _commands;
}
