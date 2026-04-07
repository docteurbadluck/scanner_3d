#include "../MotorDC_Driver.hpp"

bool MotorDC_Driver::_wait_until_pressed(Pos pos) const
{
    const uint32_t start = _io.now_ms();

    while ((_io.now_ms() - start) <= _cfg.timeout_ms)
    {
        if (pos == Pos::UP && _io.is_up_pressed())
            return true;
        if (pos == Pos::DOWN && _io.is_down_pressed())
            return true;
        _io.sleep_ms(_cfg.poll_interval_ms);
    }
    return false;
}
