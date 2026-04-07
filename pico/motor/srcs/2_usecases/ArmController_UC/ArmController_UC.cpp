#include "ArmController_UC.hpp"

ArmController_UC::ArmController_UC(IMotorDC &motor) : _motor(motor), _current_pos(Pos::UNKNOWN)
{}
