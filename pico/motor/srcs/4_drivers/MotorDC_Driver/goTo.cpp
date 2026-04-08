#include "MotorDC_Driver.hpp"

bool MotorDC_Driver::goTo(Pos pos)
{
    if (pos == Pos::UNKNOWN)
        return false;
    if (pos == Pos::UP)
        return _goUp();
    return _goDown();
}
