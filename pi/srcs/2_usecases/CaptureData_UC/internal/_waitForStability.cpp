#include "../CaptureData_UC.hpp"

bool CaptureData_UC::_waitForStability()
{
    for (int attempts = 0; attempts < MAX_STABILITY_ATTEMPTS; ++attempts)
    {
        if (_vibration.isStable())
            return true;
        _sleep_ms(STABILITY_RETRY_DELAY_MS);
    }
    return false;
}
