#pragma once

class IStepperMotor
{
	public:
		virtual ~IStepperMotor() = default;
		virtual bool	go_initial_pos() = 0;
		virtual bool	step_forward()   = 0;
};
