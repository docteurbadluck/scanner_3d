#include "../ExecuteCommand_UC.hpp"

int ExecuteCommand_UC::_execute_hardware_test()
{
    const HardwareTestResult res = _testHardware.execute();
    _sender.sendTestHardwareResult(res.dc_ok, res.servo_ok, res.stepper_ok);
    return res.dc_ok && res.servo_ok && res.stepper_ok ? 0 : 1;
}
