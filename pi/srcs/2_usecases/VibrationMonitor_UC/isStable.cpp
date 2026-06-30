#include "VibrationMonitor_UC.hpp"

bool VibrationMonitor_UC::isStable()
{
    for (int i = 0; i < STABLE_READS_REQUIRED; ++i)
    {
        if (!_isReadingStable())
            return false;
        _sleep_ms(STABLE_CHECK_DELAY_MS);
    }
    return true;
}
