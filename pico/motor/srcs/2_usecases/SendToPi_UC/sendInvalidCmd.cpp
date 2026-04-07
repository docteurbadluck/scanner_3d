#include "SendToPi_UC.hpp"

#include "1_domain/JsonMessage/JsonMessage.hpp"

void SendToPi_UC::sendInvalidCmd()
{
    _sender.send(JsonMessage::makeError("INVALID_CMD"));
}
