#include "CommandReceptor_UC.hpp"

bool CommandReceptor_UC::waitAndInterprete(System &sys)
{
    _setMessageFrom(_computer.waitForOrder());
    if (!_isValidCommand(sys.getCommands()))
    {
        _sender.sendInvalidCmd();
        return false;
    }
    sys.setCommandToExecute(_message);
    _sender.sendState(sys);
    return true;
}
