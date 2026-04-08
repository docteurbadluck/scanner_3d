#include "CommandReceptorUC.hpp"

#include "1_domain/JsonMessage/JsonMessage.hpp"

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
