#include "../ExecuteCommand_UC.hpp"

int ExecuteCommand_UC::_execute_hardware_test()
{
    const HardwareTestResult res = _testHardware.execute();
    _sender.sendTestHardwareResult(res.dc, res.servo, res.stepper);
    return (res.dc == SelfTestResult::OK
         && res.servo == ServoSelfTestResult::OK
         && res.stepper == StepperSelfTestResult::OK) ? 0 : 1;
}
