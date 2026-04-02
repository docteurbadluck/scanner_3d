#pragma once
#include "3_interface/IMotorDC.hpp"

class ArmController_UC
{
private:
    IMotorDC &_motor;
    Pos       _current_pos;

public:
    ArmController_UC(IMotorDC &motor);
    ~ArmController_UC() = default;

    bool joinInitialPos();
    bool joinPos(Pos pos);
    Pos  getPos() const;
};
