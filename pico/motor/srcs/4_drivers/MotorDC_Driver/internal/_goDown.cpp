#include "../MotorDC_Driver.hpp"

WaitResult MotorDC_Driver::_goDown()
{
    if (_io.is_down_pressed())
    {
        _io.stop();
        return WaitResult::REACHED;
    }
    _zeroBaseline = _measureBaseline();
    _io.drive_down(_cfg.speed_percent);
    const WaitResult result = _wait_until_pressed(Pos::DOWN);
    _io.stop();
    return result;
}
