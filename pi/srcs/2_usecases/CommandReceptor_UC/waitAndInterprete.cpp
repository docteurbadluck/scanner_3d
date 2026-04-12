#include "CommandReceptor_UC.hpp"

bool CommandReceptor_UC::waitAndInterprete(System &sys)
{
    _setMessageFrom(_computer.waitForOrder());
    if (!_isValidCommand(sys.getCommands()))
    {
        _logCommand(false);
        _sender.sendInvalidCmd();
        return false;
    }
    sys.setCommandToExecute(_message);
    _logCommand(true);
    _sender.sendState(sys);
    return true;
}
