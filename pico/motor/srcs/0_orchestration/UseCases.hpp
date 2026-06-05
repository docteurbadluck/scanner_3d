#pragma once

#include "0_orchestration/Drivers.hpp"
#include "1_domain/System/System.hpp"
#include "2_usecases/ArmController_UC/ArmController_UC.hpp"
#include "2_usecases/HandController_UC/HandController_UC.hpp"
#include "2_usecases/PlateController_UC/PlateController_UC.hpp"
#include "2_usecases/VibrationMonitor_UC/VibrationMonitor_UC.hpp"
#include "2_usecases/SendToPi_UC/SendToPi_UC.hpp"
#include "2_usecases/CommandReceptorUC/CommandReceptorUC.hpp"
#include "2_usecases/ExecuteCommand_UC/ExecuteCommand_UC.hpp"

struct UseCases
{
    SendToPi_UC         sender;
    CommandReceptorUC   cmdReceptor;
    ArmController_UC    arm;
    HandController_UC   hand;
    PlateController_UC  plate;
    VibrationMonitor_UC vibration;
    ExecuteCommand_UC   executor;

    explicit UseCases(Drivers &drv) :
        sender(drv.uartSender),
        cmdReceptor(drv.receptor, Commands{}, sender),
        arm(drv.motorDC), hand(drv.servo), plate(drv.stepper),
        vibration(drv.acc1, drv.acc2),
        executor(arm, hand, plate, vibration, sender)
    {}
};
