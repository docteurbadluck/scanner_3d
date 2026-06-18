#pragma once
#include "3_interface/IAccelerometer.hpp"
#include <cstdint>
#include <functional>

class VibrationMonitor_UC
{
private:
    IAccelerometer                &_acc1;
    IAccelerometer                &_acc2;
    std::function<void(uint32_t)>  _sleep_ms;

    static constexpr float    VIBRATION_THRESHOLD   = 0.05f;
    static constexpr int      STABLE_READS_REQUIRED = 5;
    static constexpr uint32_t STABLE_CHECK_DELAY_MS  = 20;

    bool _isReadingStable();

public:
    VibrationMonitor_UC(IAccelerometer &acc1, IAccelerometer &acc2, std::function<void(uint32_t)> sleep_ms);
    ~VibrationMonitor_UC() = default;

    bool isStable();
};
