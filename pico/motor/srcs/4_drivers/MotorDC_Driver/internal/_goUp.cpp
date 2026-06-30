#include "../MotorDC_Driver.hpp"

WaitResult MotorDC_Driver::_goUp()
{
    if (_io.is_up_pressed())
    {
        _io.stop();
        return WaitResult::REACHED;
    }
    _zeroBaseline = _measureBaseline();
    _io.drive_up(_cfg.speed_percent);
    const WaitResult result = _wait_until_pressed(Pos::UP);
    _io.stop();
    return result;
}
