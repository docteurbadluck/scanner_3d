#pragma once

enum class Pos
{
    UP,
    DOWN,
    UNKNOWN,
};

class IMotorDC
{
public:
    virtual ~IMotorDC() = default;
    virtual bool goInitialPos() = 0;
    virtual bool goTo(Pos pos) = 0;
};
