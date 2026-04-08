#include "ServoMotor_Driver.hpp"

namespace
{
static constexpr float PULSE_MIN_US  = 500.0f;
static constexpr float PULSE_RANGE   = 2000.0f;
static constexpr float ANGLE_MAX_DEG = 180.0f;
}

uint16_t ServoMotor_Driver::_angleToPulse(float angle_deg) const
{
    return static_cast<uint16_t>(PULSE_MIN_US + (angle_deg / ANGLE_MAX_DEG) * PULSE_RANGE);
}
