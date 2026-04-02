#pragma once
#include "3_interface/IServoMotor.hpp"

class HandController_UC
{
private:
    IServoMotor &_servo;
    Pos_hand     _current_pos;

public:
    HandController_UC(IServoMotor &servo);
    ~HandController_UC() = default;

    bool     joinInitialPos();
    bool     joinPos(Pos_hand pos);
    Pos_hand getPos() const;
};
