#include "System.hpp"

System::System()
{
    _current_state = LISTENING;
}

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

void System::commandReceived()
{
    _current_state = INTERPRETING;
}

void System::commandInterpreted(bool correct)
{
    if (correct)
    {
        _current_state = EXECUTING;
    }
    else
    {
        _current_state = LISTENING;
    }
}

bool System::resultCommand(int res_code)
{
    _current_state = LISTENING;
    return (res_code == 0);
}

void System::setCommandToExecute(const std::string &command)
{
    _command_to_execute = command;
}

std::string System::getCommandToExecute() const
{
    return _command_to_execute;
}

Commands System::getCommands() const
{
    return _commands;
}
