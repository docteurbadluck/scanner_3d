#include "HandController_UC.hpp"

HandController_UC::HandController_UC(IServoMotor &servo) : _servo(servo), _current_pos(Pos_hand::UNKNOWN)
{}

bool HandController_UC::joinInitialPos()
{
    if (_servo.goInitialPos())
    {
        _current_pos = Pos_hand::UP_A;
        return true;
    }
    _current_pos = Pos_hand::UNKNOWN;
    return false;
}

bool HandController_UC::joinPos(Pos_hand pos)
{
    if (_current_pos == pos)
        return true;
    if (_servo.goTo(pos))
    {
        _current_pos = pos;
        return true;
    }
    _current_pos = Pos_hand::UNKNOWN;
    return false;
}

Pos_hand HandController_UC::getPos() const
{
    return _current_pos;
}
