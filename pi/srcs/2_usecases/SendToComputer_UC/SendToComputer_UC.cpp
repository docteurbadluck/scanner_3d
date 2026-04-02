#include "SendToComputer_UC.hpp"

SendToComputer_UC::SendToComputer_UC(ISender &sender) : _sender(sender)
{}

void SendToComputer_UC::sendState(const System &sys)
{
    _sender.send(sys.getStateString());
}

void SendToComputer_UC::sendInvalidCmd()
{
    _sender.send("INVALID_CMD");
}

void SendToComputer_UC::sendRaw(const std::string &message)
{
    _sender.send(message);
}

void SendToComputer_UC::sendResponse(const std::string &cmd, bool success, const System &sys)
{
    if (cmd == "PING")
        _sender.send("PONG");
    else if (cmd == "GET_STATUS")
        _sender.send(sys.getStateString());
    else if (success)
        _sender.send("DONE");
    else
        _sender.send("FAIL");
}
