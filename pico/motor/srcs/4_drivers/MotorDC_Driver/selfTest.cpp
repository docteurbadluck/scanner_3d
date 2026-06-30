#include "MotorDC_Driver.hpp"

namespace
{
SelfTestResult toSelfTestResult(
    WaitResult result, SelfTestResult unreachable, SelfTestResult stalled)
{
    if (result == WaitResult::STALLED)
        return stalled;
    if (result == WaitResult::TIMEOUT)
        return unreachable;
    return SelfTestResult::OK;
}
}

SelfTestResult MotorDC_Driver::selfTest()
{
    const WaitResult downResult = _goDown();
    SelfTestResult   res = toSelfTestResult(downResult, SelfTestResult::DOWN_UNREACHABLE,
        SelfTestResult::DOWN_STALLED);
    if (downResult == WaitResult::REACHED)
    {
        const WaitResult upResult = _goUp();
        res = toSelfTestResult(
            upResult, SelfTestResult::UP_UNREACHABLE, SelfTestResult::UP_STALLED);
    }
    stop();
    return res;
}
