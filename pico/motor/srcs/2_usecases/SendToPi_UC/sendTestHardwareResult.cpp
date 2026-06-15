#include "SendToPi_UC.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

void SendToPi_UC::sendTestHardwareResult(bool dc_ok, bool servo_ok, bool stepper_ok)
{
    _sender.send(JsonMessage::makeTestHardwareResponse(dc_ok, servo_ok, stepper_ok));
}
