#include "ServoMotor_Driver.hpp"

float ServoMotor_Driver::_angleForPos(Pos_hand pos) const
{
    if (pos == Pos_hand::UP_A)   return _cfg.angle_up_a_deg;
    if (pos == Pos_hand::UP_B)   return _cfg.angle_up_b_deg;
    if (pos == Pos_hand::DOWN_A) return _cfg.angle_down_a_deg;
    return _cfg.angle_down_b_deg;
}
