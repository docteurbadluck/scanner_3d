#include "HandController_UC.hpp"

HandController_UC::HandController_UC(IServoMotor &servo) : _servo(servo), _current_pos(Pos_hand::UNKNOWN)
{}
