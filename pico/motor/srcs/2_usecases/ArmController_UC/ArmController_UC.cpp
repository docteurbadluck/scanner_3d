#include "ArmController_UC.hpp"

ArmController_UC::ArmController_UC(IMotorDC &motor) : _motor(motor), _current_pos(UNKNOWN)
{
}

bool ArmController_UC::join_initial_pos()
{
	if (_motor.go_initial_pos())
	{
		_current_pos = UP;
		return true;
	}
	_current_pos = UNKNOWN;
	return false;
}

bool ArmController_UC::join_pos(Pos pos)
{
	if (_current_pos == pos)
		return true;
	if (_motor.go_to(pos))
	{
		_current_pos = pos;
		return true;
	}
	_current_pos = UNKNOWN;
	return false;
}

