#include "ServoMotor_Driver.hpp"

#include <pico/stdlib.h>

bool ServoMotor_Driver::goTo(Pos_hand pos)
{
    if (pos == Pos_hand::UNKNOWN)
        return false;

    const float    angle = _angleForPos(pos);
    const uint16_t pulse = _angleToPulse(angle);
    _setPulse(pulse);
    ::sleep_ms(_cfg.move_delay_ms);
    return true;
}
