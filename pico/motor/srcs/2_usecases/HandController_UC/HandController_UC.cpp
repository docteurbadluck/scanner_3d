#include "HandController_UC.hpp"

HandController_UC::HandController_UC(IServoMotor &motor) : _servo(motor), _current_pos(UNKNOWN)
{
}

bool HandController_UC::join_initial_pos()
{
	if (_servo.go_initial_pos())
	{
		_current_pos = UP_A;
		return true;
	}
	_current_pos = UNKNOWN;
	return false;
}

bool HandController_UC::join_pos(Pos_hand pos)
{
	if (_current_pos == pos)
		return true;
	if (_servo.go_to(pos))
	{
		_current_pos = pos;
		return true;
	}
	_current_pos = UNKNOWN;
	return false;
}

