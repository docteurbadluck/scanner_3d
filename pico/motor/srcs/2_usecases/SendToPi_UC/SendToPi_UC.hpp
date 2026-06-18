#pragma once
#include <string>
#include "1_domain/System/System.hpp"
#include "3_interface/ISender.hpp"
#include "3_interface/IMotorDC.hpp"
#include "3_interface/IServoMotor.hpp"
#include "3_interface/IStepperMotor.hpp"

class SendToPi_UC
{
private:
    ISender &_sender;

public:
    SendToPi_UC(ISender &sender);
    ~SendToPi_UC() = default;

    void sendResponse(const std::string &cmd, bool success, const System &sys);
    void sendTestHardwareResult(SelfTestResult dc, ServoSelfTestResult servo, StepperSelfTestResult stepper);
    void sendState(const System &sys);
    void sendInvalidCmd();
};
