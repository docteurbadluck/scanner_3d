#include "../MotorDC_Driver.hpp"

bool MotorDC_Driver::_isTargetPressed(Pos pos) const
{
    if (pos == Pos::UP)
        return _io.is_up_pressed();
    return _io.is_down_pressed();
}
