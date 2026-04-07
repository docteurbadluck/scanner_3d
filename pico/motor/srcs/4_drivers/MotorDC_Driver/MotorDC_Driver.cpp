#include "MotorDC_Driver.hpp"

MotorDC_Driver::MotorDC_Driver(const MotorDC_DriverConfig &cfg, MotorDC_DriverIO io)
    : _cfg(cfg), _io(std::move(io))
{
}

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

bool MotorDC_Driver::goInitialPos()
{
    return goTo(Pos::UP);
}

bool MotorDC_Driver::goTo(Pos pos)
{
    if (pos == Pos::UNKNOWN)
        return false;

    if (pos == Pos::UP)
    {
        if (_io.is_up_pressed())
        {
            _io.stop();
            return true;
        }

        _io.drive_up(_cfg.speed_percent);
        const bool reached = _wait_until_pressed(Pos::UP);
        _io.stop();
        return reached;
    }

    if (_io.is_down_pressed())
    {
        _io.stop();
        return true;
    }

    _io.drive_down(_cfg.speed_percent);
    const bool reached = _wait_until_pressed(Pos::DOWN);
    _io.stop();
    return reached;
}

