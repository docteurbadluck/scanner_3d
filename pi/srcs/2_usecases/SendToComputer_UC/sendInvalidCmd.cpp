#include "SendToComputer_UC.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

void SendToComputer_UC::sendInvalidCmd()
{
    _sender.send(JsonMessage::makeError("INVALID_CMD"));
}
