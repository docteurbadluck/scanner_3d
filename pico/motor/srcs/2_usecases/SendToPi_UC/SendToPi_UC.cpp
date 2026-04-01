#include "SendToPi_UC.hpp"

SendToPi_UC::SendToPi_UC(ISender &sender) : _sender(sender)
{}

void SendToPi_UC::sendState(const System &sys)
{
    _sender.send(sys.getStateString());
}

void SendToPi_UC::sendInvalidCmd()
{
    _sender.send("INVALID_CMD");
}

void SendToPi_UC::sendResponse(const std::string &cmd, bool success, const System &sys)
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
