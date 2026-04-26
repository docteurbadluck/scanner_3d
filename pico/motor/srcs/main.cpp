#include <stdio.h>
#include <pico/stdlib.h>

#include "0_orchestration/config.hpp"

#include "1_domain/System/System.hpp"
#include "2_usecases/ArmController_UC/ArmController_UC.hpp"
#include "2_usecases/HandController_UC/HandController_UC.hpp"
#include "2_usecases/PlateController_UC/PlateController_UC.hpp"
#include "2_usecases/VibrationMonitor_UC/VibrationMonitor_UC.hpp"
#include "2_usecases/SendToPi_UC/SendToPi_UC.hpp"
#include "2_usecases/CommandReceptorUC/CommandReceptorUC.hpp"
#include "2_usecases/ExecuteCommand_UC/ExecuteCommand_UC.hpp"

struct Drivers
{
    UartReceptor_Driver  receptor;
    UartSender_Driver    uartSender;
    MotorDC_Driver       motorDC;
    ServoMotor_Driver    servo;
    StepperMotor_Driver  stepper;
    Accelerometer_Driver acc1;
    Accelerometer_Driver acc2;

    Drivers() :
        receptor(UART_RECEPTOR_CONFIG), uartSender(),
        motorDC(MOTOR_DC_CONFIG, MOTOR_DC_PINS),
        servo(SERVO_CONFIG, SERVO_PINS),
        stepper(STEPPER_CONFIG, STEPPER_PINS),
        acc1(ACC1_CONFIG, ACC_PINS), acc2(ACC2_CONFIG, ACC_PINS)
    {}
};

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

static void run_loop(UseCases &uc, System &sys)
{
    while (true)
    {
        uc.cmdReceptor.checkForNewMessage(sys);
        if (sys.getState() == INTERPRETING)
            uc.cmdReceptor.interpreteMessage(sys);
        if (sys.getState() == EXECUTING)
            uc.executor.execute(sys);
    }
}

int main()
{
    stdio_init_all();
    Drivers  drv;
    UseCases uc(drv);
    System   sys;
    run_loop(uc, sys);
    return 0;
}
