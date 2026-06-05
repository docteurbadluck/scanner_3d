#include "MotorDC_Driver.hpp"

SelfTestResult MotorDC_Driver::selfTest()
{
    SelfTestResult res = SelfTestResult::OK;
    if (!_goDown())
        res = SelfTestResult::DOWN_UNREACHABLE;
    else if (!_goUp())
        res = SelfTestResult::UP_UNREACHABLE;
    stop();
    return res;
}
