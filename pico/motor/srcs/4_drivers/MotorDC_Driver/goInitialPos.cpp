#include "MotorDC_Driver.hpp"

bool MotorDC_Driver::goInitialPos()
{
    return goTo(Pos::UP);
}
