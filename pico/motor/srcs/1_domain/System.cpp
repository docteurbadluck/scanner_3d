#include "System.hpp"


System::System()
{
    _current_state = LISTENING;
}

state System::get_state() const 
{
    return _current_state;
}
void System::command_received()
{
    _current_state = INTERPRETING;
}

void System::command_interpreted(bool correct)
{
    if (correct)
    _current_state = EXECUTING;
    else 
    _current_state = LISTENING;
}

bool System::result_command(int res_code)
{
    _current_state = LISTENING;
    return (res_code == 0);
}

void System::set_command_to_execute(const std::string &command)
{
    _command_to_execute = command;
}        

Commands System::get_commands() const
{
    return _commands;
}
