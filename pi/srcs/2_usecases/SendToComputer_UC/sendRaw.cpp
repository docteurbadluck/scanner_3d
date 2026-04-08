#include "SendToComputer_UC.hpp"

void SendToComputer_UC::sendRaw(const std::string &message)
{
    _sender.send(message);
}
