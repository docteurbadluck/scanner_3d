#include "ControllerDC.hpp"

ControllerDC::ControllerDC(IMotorDC &motor) : _motor(motor), _current_pos(UNKNOWN)
{
}

bool ControllerDC::join_initial_pos()
{
	if (_motor.go_initial_pos())
	{
		_current_pos = UP;
		return true;
	}
	_current_pos = UNKNOWN;
	return false;
}

bool ControllerDC::join_pos(Pos pos)
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

