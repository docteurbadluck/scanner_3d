#pragma once
#include "3_interface/IAccelerometer.hpp"

class VibrationMonitor_UC
{
	private:
		IAccelerometer	&_acc1;
		IAccelerometer	&_acc2;

		static constexpr float VIBRATION_THRESHOLD = 0.05f;

	public:
		VibrationMonitor_UC(IAccelerometer &acc1, IAccelerometer &acc2);
		~VibrationMonitor_UC() = default;

		bool	isStable();
};
