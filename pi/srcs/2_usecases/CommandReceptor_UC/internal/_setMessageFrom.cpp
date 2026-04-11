#include "../CommandReceptor_UC.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

void CommandReceptor_UC::_setMessageFrom(const std::string &raw)
{
    const std::string jsonCommand = JsonMessage::extractStringField(raw, "command");
    _message = jsonCommand.empty() ? raw : jsonCommand;
}
