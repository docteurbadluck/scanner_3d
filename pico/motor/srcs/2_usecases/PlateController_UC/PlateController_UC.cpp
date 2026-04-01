#include "PlateController_UC.hpp"

PlateController_UC::PlateController_UC(IStepperMotor &stepper)
	: _stepper(stepper), _current_step(-1)
{
}

bool PlateController_UC::join_initial_pos()
{
	if (_stepper.go_initial_pos())
	{
		_current_step = 0;
		return true;
	}
	_current_step = -1;
	return false;
}

bool PlateController_UC::_rotate_steps(int steps)
{
	for (int i = 0; i < steps; i++)
	{
		if (!_stepper.step_forward())
		{
			_current_step = -1;
			return false;
		}
		_current_step = (_current_step + 1) % STEPS_PER_REVOLUTION;
	}
	return true;
}

bool PlateController_UC::rotate_to(int target_step)
{
	if (_current_step < 0)
		return false;
	if (_current_step == target_step)
		return true;
	int steps = (target_step - _current_step + STEPS_PER_REVOLUTION) % STEPS_PER_REVOLUTION;
	return _rotate_steps(steps);
}
