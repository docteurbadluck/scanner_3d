#include "SendToPi_UC.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

static std::string dc_to_str(SelfTestResult r)
{
    if (r == SelfTestResult::DOWN_UNREACHABLE) return "DOWN_UNREACHABLE";
    if (r == SelfTestResult::UP_UNREACHABLE)   return "UP_UNREACHABLE";
    if (r == SelfTestResult::DOWN_STALLED)     return "DOWN_STALLED";
    if (r == SelfTestResult::UP_STALLED)       return "UP_STALLED";
    return "OK";
}

void SendToPi_UC::sendTestHardwareResult(SelfTestResult dc, ServoSelfTestResult servo,
    StepperSelfTestResult stepper)
{
    const std::string servo_str   = servo   == ServoSelfTestResult::OK   ? "OK" : "NO_CURRENT";
    const std::string stepper_str = stepper == StepperSelfTestResult::OK ? "OK" : "FAIL";
    _sender.send(JsonMessage::makeTestHardwareResponse(dc_to_str(dc), servo_str, stepper_str));
}