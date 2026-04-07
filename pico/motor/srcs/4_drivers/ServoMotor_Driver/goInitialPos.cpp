#include "ServoMotor_Driver.hpp"

bool ServoMotor_Driver::goInitialPos()
{
    return goTo(Pos_hand::UP_A);
}
