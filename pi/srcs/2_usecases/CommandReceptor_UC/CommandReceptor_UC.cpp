#include "CommandReceptor_UC.hpp"
#include "1_domain/JsonMessage/JsonMessage.hpp"

CommandReceptor_UC::CommandReceptor_UC(IComputer &uploader, SendToComputer_UC &sender)
    : _computer(uploader), _sender(sender)
{}

void CommandReceptor_UC::_setMessageFrom(const std::string &raw)
{
    const std::string jsonCommand = JsonMessage::extractStringField(raw, "command");
    _message = jsonCommand.empty() ? raw : jsonCommand;
}

bool CommandReceptor_UC::_isValidCommand(const Commands &cmds) const
{
    for (size_t i = 0; i < cmds.valid_command.size(); i++)
    {
        if (_message == cmds.valid_command[i])
            return true;
    }
    return false;
}

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

std::string CommandReceptor_UC::getMessage() const
{
    return _message;
}
