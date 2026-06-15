#include "VibrationMonitor_UC.hpp"

bool VibrationMonitor_UC::isStable()
{
    const float mag1 = _acc1.readMagnitude();
    const float mag2 = _acc2.readMagnitude();
    return mag1 < VIBRATION_THRESHOLD && mag2 < VIBRATION_THRESHOLD;
}
