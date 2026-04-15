#include "SendToComputer_UC.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

void SendToComputer_UC::sendPicoStatus(const std::string &rawPicoJson)
{
    const std::string state = JsonMessage::extractStringField(rawPicoJson, "state");
    _sender.send(JsonMessage::makePicoStatus(state.empty() ? "unknown" : state));
}
