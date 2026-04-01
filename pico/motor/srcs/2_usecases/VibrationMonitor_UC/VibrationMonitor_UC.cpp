#include "VibrationMonitor_UC.hpp"

VibrationMonitor_UC::VibrationMonitor_UC(IAccelerometer &acc1, IAccelerometer &acc2)
	: _acc1(acc1), _acc2(acc2)
{
}

bool VibrationMonitor_UC::isStable()
{
	float mag1 = _acc1.read_magnitude();
	float mag2 = _acc2.read_magnitude();
	return mag1 < VIBRATION_THRESHOLD && mag2 < VIBRATION_THRESHOLD;
}
