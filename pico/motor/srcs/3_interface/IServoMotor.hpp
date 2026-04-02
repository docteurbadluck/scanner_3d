#pragma once

enum class Pos_hand
{
    UP_A,
    UP_B,
    DOWN_A,
    DOWN_B,
    UNKNOWN,
};

class IServoMotor
{
public:
    virtual ~IServoMotor() = default;
    virtual bool goInitialPos() = 0;
    virtual bool goTo(Pos_hand pos) = 0;
};
