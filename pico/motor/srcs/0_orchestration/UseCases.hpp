#pragma once

#include "0_orchestration/Drivers.hpp"
#include "1_domain/System/System.hpp"
#include "2_usecases/ArmController_UC/ArmController_UC.hpp"
#include "2_usecases/HandController_UC/HandController_UC.hpp"
#include "2_usecases/PlateController_UC/PlateController_UC.hpp"
#include "2_usecases/SendToPi_UC/SendToPi_UC.hpp"
#include "2_usecases/CommandReceptorUC/CommandReceptorUC.hpp"
#include "2_usecases/TestHardware_UC/TestHardware_UC.hpp"
#include "2_usecases/ExecuteCommand_UC/ExecuteCommand_UC.hpp"

struct UseCases
{
    SendToPi_UC        sender;
    CommandReceptorUC  cmdReceptor;
    ArmController_UC   arm;
    HandController_UC  hand;
    PlateController_UC plate;
    TestHardware_UC    testHardware;
    ExecuteCommand_UC  executor;

    explicit UseCases(Drivers &drv) :
        sender(drv.uartSender),
        cmdReceptor(drv.receptor, Commands{}, sender),
        arm(drv.motorDC), hand(drv.servo), plate(drv.stepper),
        testHardware(drv.motorDC, drv.servo, drv.stepper),
        executor(arm, hand, plate, sender, testHardware)
    {}
};
