#include "TestHardware_UC.hpp"

TestHardware_UC::TestHardware_UC(IMotorDC &dc, IServoMotor &servo, IStepperMotor &stepper)
    : _dc(dc), _servo(servo), _stepper(stepper)
{}

