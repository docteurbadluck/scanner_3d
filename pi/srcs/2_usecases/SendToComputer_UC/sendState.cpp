#include "SendToComputer_UC.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

void SendToComputer_UC::sendState(const System &sys)
{
    _sender.send(JsonMessage::makeState(sys.getStateString()));
}
