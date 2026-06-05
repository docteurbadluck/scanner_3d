#pragma once

enum class Pos
{
    UP,
    DOWN,
    UNKNOWN,
};

enum class SelfTestResult
{
    OK,
    DOWN_UNREACHABLE,
    UP_UNREACHABLE,
};

class IMotorDC
{
public:
    virtual ~IMotorDC() = default;
    virtual void           stop() = 0;
    virtual bool           goInitialPos() = 0;
    virtual bool           goTo(Pos pos) = 0;
    virtual SelfTestResult selfTest() = 0;
};
