#pragma once
#include "3_interface/IStepperMotor.hpp"

class PlateController_UC
{
	private:
		IStepperMotor	&_stepper;
		int				_current_step;

		static constexpr int STEPS_PER_REVOLUTION = 36;

		bool	_rotate_steps(int steps);

	public:
		PlateController_UC(IStepperMotor &stepper);
		~PlateController_UC() = default;

		bool	join_initial_pos();
		bool	rotate_to(int target_step);
		int		get_pos() const;
};
