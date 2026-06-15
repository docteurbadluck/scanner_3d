#pragma once
#include "1_domain/System/System.hpp"
#include "2_usecases/ArmController_UC/ArmController_UC.hpp"
#include "2_usecases/HandController_UC/HandController_UC.hpp"
#include "2_usecases/PlateController_UC/PlateController_UC.hpp"
#include "2_usecases/SendToPi_UC/SendToPi_UC.hpp"
#include "2_usecases/TestHardware_UC/TestHardware_UC.hpp"

class ExecuteCommand_UC
{
private:
    ArmController_UC   &_arm;
    HandController_UC  &_hand;
    PlateController_UC &_plate;
    SendToPi_UC        &_sender;
    TestHardware_UC    &_testHardware;

    int _dispatch(const std::string &cmd);
    int _dispatchArm(const std::string &cmd);
    int _dispatchHand(const std::string &cmd);
    int _dispatchSystem(const std::string &cmd);
    int _moveArmTo(Pos pos);
    int _moveCamTo(Pos_hand pos);
    int _goInitialPos();
    int _execute_hardware_test();

public:
    ExecuteCommand_UC(ArmController_UC &arm, HandController_UC &hand,
                      PlateController_UC &plate,
                      SendToPi_UC &sender, TestHardware_UC &testHardware);
    ~ExecuteCommand_UC() = default;
    void execute(System &sys);
};
