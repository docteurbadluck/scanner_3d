#include "ArmController_UC.hpp"

ArmController_UC::ArmController_UC(IMotorDC &motor) : _motor(motor), _current_pos(Pos::UNKNOWN)
{}

bool ArmController_UC::joinInitialPos()
{
    if (_motor.goInitialPos())
    {
        _current_pos = Pos::UP;
        return true;
    }
    _current_pos = Pos::UNKNOWN;
    return false;
}

bool ArmController_UC::joinPos(Pos pos)
{
    if (_current_pos == pos)
        return true;
    if (_motor.goTo(pos))
    {
        _current_pos = pos;
        return true;
    }
    _current_pos = Pos::UNKNOWN;
    return false;
}

Pos ArmController_UC::getPos() const
{
    return _current_pos;
}
