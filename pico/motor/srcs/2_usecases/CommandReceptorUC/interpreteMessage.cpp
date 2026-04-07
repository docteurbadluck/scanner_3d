#include "CommandReceptorUC.hpp"

bool CommandReceptorUC::interpreteMessage(System &sys)
{
    for (size_t i = 0; i < _cmds.valid_command.size(); i++)
    {
        if (_message == _cmds.valid_command[i])
        {
            _handleValidCommand(sys);
            return true;
        }
    }
    sys.commandInterpreted(false);
    _sender.sendInvalidCmd();
    return false;
}
