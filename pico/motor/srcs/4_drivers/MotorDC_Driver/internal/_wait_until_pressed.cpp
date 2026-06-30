#include "../MotorDC_Driver.hpp"

WaitResult MotorDC_Driver::_wait_until_pressed(Pos pos) const
{
    const uint32_t start = _io.now_ms();
    uint8_t         overThresholdCount = 0;

    while ((_io.now_ms() - start) <= _cfg.timeout_ms)
    {
        if (_isTargetPressed(pos))
            return WaitResult::REACHED;
        const bool pastStartupGrace = (_io.now_ms() - start) >= _cfg.stall_grace_ms;
        if (pastStartupGrace && _isStalled(overThresholdCount))
            return WaitResult::STALLED;
        _io.sleep_ms(_cfg.poll_interval_ms);
    }
    return WaitResult::TIMEOUT;
}
