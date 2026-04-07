#include "../MotorDC_Driver.hpp"

bool MotorDC_Driver::_goDown()
{
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
