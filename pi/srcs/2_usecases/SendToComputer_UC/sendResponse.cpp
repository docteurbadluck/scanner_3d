#include "SendToComputer_UC.hpp"
#include "1_domain/JsonMessage/JsonMessage.hpp"

void SendToComputer_UC::sendResponse(const std::string &cmd, bool success, const System &sys)
{
    if (cmd == "PING")
        _sender.send(JsonMessage::makeResponse("PONG", cmd));
    else if (cmd == "GET_STATUS")
        _sender.send(JsonMessage::makeState(sys.getStateString()));
    else if (success)
        _sender.send(JsonMessage::makeResponse("DONE", cmd));
    else
        _sender.send(JsonMessage::makeResponse("FAIL", cmd));
}
