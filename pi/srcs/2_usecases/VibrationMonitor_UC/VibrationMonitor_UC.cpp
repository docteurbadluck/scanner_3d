#include "VibrationMonitor_UC.hpp"

VibrationMonitor_UC::VibrationMonitor_UC(
    IAccelerometer &acc1, IAccelerometer &acc2, std::function<void(uint32_t)> sleep_ms)
    : _acc1(acc1), _acc2(acc2), _sleep_ms(sleep_ms)
{}
