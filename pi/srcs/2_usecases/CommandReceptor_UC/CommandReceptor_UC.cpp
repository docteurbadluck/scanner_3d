#include "CommandReceptor_UC.hpp"

CommandReceptor_UC::CommandReceptor_UC(IComputer &uploader, SendToComputer_UC &sender)
    : _computer(uploader), _sender(sender)
{}

bool CommandReceptor_UC::waitAndInterprete(System &sys)
{
    _message = _computer.waitForOrder();
    const Commands& cmds = sys.getCommands();
    for (size_t i = 0; i < cmds.valid_command.size(); i++)
    {
        if (_message == cmds.valid_command[i])
        {
            sys.setCommandToExecute(_message);
            _sender.sendState(sys);
            return true;
        }
    }
    _sender.sendInvalidCmd();
    return false;
}

std::string CommandReceptor_UC::getMessage() const
{
    return _message;
}
