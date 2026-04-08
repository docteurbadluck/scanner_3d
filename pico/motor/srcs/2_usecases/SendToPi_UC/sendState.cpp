#include "SendToPi_UC.hpp"

#include "3_interface/JsonMessage/JsonMessage.hpp"

void SendToPi_UC::sendState(const System &sys)
{
    _sender.send(JsonMessage::makeState(sys.getStateString()));
}
