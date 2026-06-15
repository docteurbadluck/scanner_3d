#include "VibrationMonitor_UC.hpp"

VibrationMonitor_UC::VibrationMonitor_UC(IAccelerometer &acc1, IAccelerometer &acc2)
    : _acc1(acc1), _acc2(acc2)
{}
