#include "../MotorDC_Driver.hpp"

bool MotorDC_Driver::_goUp()
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
