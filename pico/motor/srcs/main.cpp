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

int main()
{
    stdio_init_all();

    // ── Drivers ───────────────────────────────────────────────────────────
    UsbReceptor_Driver   receptor(USB_RECEPTOR_CONFIG);
    UsbSender_Driver     usbSender;
    MotorDC_Driver       motorDC(MOTOR_DC_CONFIG, MOTOR_DC_PINS);
    ServoMotor_Driver    servo(SERVO_CONFIG, SERVO_PINS);
    StepperMotor_Driver  stepper(STEPPER_CONFIG, STEPPER_PINS);
    Accelerometer_Driver acc1(ACC1_CONFIG, ACC_PINS);
    Accelerometer_Driver acc2(ACC2_CONFIG, ACC_PINS);

    // ── Use-cases ─────────────────────────────────────────────────────────
    SendToPi_UC         sender(usbSender);
    CommandReceptorUC   cmdReceptor(receptor, Commands{}, sender);
    ArmController_UC    arm(motorDC);
    HandController_UC   hand(servo);
    PlateController_UC  plate(stepper);
    VibrationMonitor_UC vibration(acc1, acc2);
    ExecuteCommand_UC   executor(arm, hand, plate, vibration, sender);

    // ── Domain ────────────────────────────────────────────────────────────
    System sys;

    // ── Loop ──────────────────────────────────────────────────────────────
    while (true)
    {
        cmdReceptor.checkForNewMessage(sys);
        if (sys.getState() == INTERPRETING)
            cmdReceptor.interpreteMessage(sys);
        if (sys.getState() == EXECUTING)
            executor.execute(sys);
    }
    return 0;
}
