#include "CommandReceptorUC.hpp"
#include "1_domain/JsonMessage/JsonMessage.hpp"

CommandReceptorUC::CommandReceptorUC(IReceptor &recep, Commands cmds, SendToPi_UC &sender)
    : _receptor(recep), _cmds(cmds), _sender(sender)
{}

void CommandReceptorUC::checkForNewMessage(System &sys)
{
    if (_receptor.isMessageArrived() && sys.getState() == LISTENING)
    {
        const std::string rawMessage = _receptor.getMessage();
        const std::string jsonCommand = JsonMessage::extractStringField(rawMessage, "command");
        _message = jsonCommand.empty() ? rawMessage : jsonCommand;
        sys.commandReceived();
        _sender.sendState(sys);
    }
}

bool CommandReceptorUC::interpreteMessage(System &sys)
{
    for (size_t i = 0; i < _cmds.valid_command.size(); i++)
    {
        if (_message == _cmds.valid_command[i])
        {
            sys.commandInterpreted(true);
            sys.setCommandToExecute(_message);
            _sender.sendState(sys);
            return true;
        }
    }
    sys.commandInterpreted(false);
    _sender.sendInvalidCmd();
    return false;
}

std::string CommandReceptorUC::getMessage()
{
    return _message;
}
