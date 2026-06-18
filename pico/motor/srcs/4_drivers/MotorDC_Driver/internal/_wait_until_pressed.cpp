#include "../MotorDC_Driver.hpp"

bool MotorDC_Driver::_wait_until_pressed(Pos pos) const
{
    const uint32_t start = _io.now_ms();

    while ((_io.now_ms() - start) <= _cfg.timeout_ms)
    {
        if (_isTargetPressed(pos))
            return true;
        if (_io.read_current() > _cfg.stall_threshold_adc)
            return false;
        _io.sleep_ms(_cfg.poll_interval_ms);
    }
    return false;
}
