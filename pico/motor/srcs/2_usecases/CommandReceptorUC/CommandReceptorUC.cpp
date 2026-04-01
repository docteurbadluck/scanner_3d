#include "CommandReceptorUC.hpp"

CommandReceptorUC::CommandReceptorUC(IReceptor &recep, Commands cmds) : _receptor(recep), _cmds(cmds)
{
}

void CommandReceptorUC::check_for_new_message(System &sys)
{
    if (_receptor.is_message_arrived() && sys.get_state() == LISTENING)
    {
        _message = _receptor.get_message();
        sys.command_received();
    }
}

bool CommandReceptorUC::interprete_message(System &sys)
{

    for (int i = 0; i < _cmds.nbr_command ; i++)
    {
        if (_message == _cmds.valid_command[i])
        {
            sys.command_interpreted(true);
            sys.set_command_to_execute(_message);
            return true;
        }
    }
    sys.command_interpreted(false);
    return false;
}

std::string CommandReceptorUC::get_message()
{
    return _message;   
}
