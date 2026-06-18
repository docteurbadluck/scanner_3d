#include "1_domain/System/System.hpp"
#include "2_usecases/CaptureData_UC/CaptureData_UC.hpp"
#include "2_usecases/CommandReceptor_UC/CommandReceptor_UC.hpp"
#include "2_usecases/ExecuteOrder_UC/ExecuteOrder_UC.hpp"
#include "2_usecases/SendPhotoToComputer_UC/SendPhotoToComputer_UC.hpp"
#include "2_usecases/SendToComputer_UC/SendToComputer_UC.hpp"
#include "2_usecases/VibrationMonitor_UC/VibrationMonitor_UC.hpp"
#include "4_drivers/Accelerometer_Driver/Accelerometer_Driver.hpp"
#include "4_drivers/Camera_Driver/Camera_Driver.hpp"
#include "4_drivers/Computer_Driver/Computer_Driver.hpp"
#include "4_drivers/DiskChecker_Driver/DiskChecker_Driver.hpp"
#include "4_drivers/Pico_Driver/Pico_Driver.hpp"
#include <unistd.h>


static void runInitialization(
    System &sys, Computer_Driver &computer, SendToComputer_UC &sender); 


static void runOneCommand(
    System &sys, CommandReceptor_UC &receptor, ExecuteOrder_UC &executor, SendToComputer_UC &sender);

static Computer_DriverConfig buildConfig(int argc, char *argv[])
{
    Computer_DriverConfig cfg;
    cfg.host = (argc >= 2) ? argv[1] : "127.0.0.1";
    return cfg;
}

static void sleep_ms(uint32_t ms)
{
    usleep(ms * 1000);
}

static void runLoop(
    System &sys, CommandReceptor_UC &receptor,
    ExecuteOrder_UC &executor, SendToComputer_UC &sender);

static void runSession(Computer_Driver &computer, Pico_Driver &pico,
    Camera_Driver &camera, DiskChecker_Driver &disk,
    Accelerometer_Driver &acc1, Accelerometer_Driver &acc2)
{
    SendToComputer_UC      sender(computer);
    CommandReceptor_UC     receptor(computer, sender);
    VibrationMonitor_UC    vibration(acc1, acc2, sleep_ms);
    CaptureData_UC         capture(camera, disk, vibration);
    SendPhotoToComputer_UC sendPhoto(camera, computer);
    ExecuteOrder_UC        executor(capture, sendPhoto, sender, pico);
    System sys;
    runInitialization(sys, computer, sender);
    runLoop(sys, receptor, executor, sender);
}

int main(int argc, char *argv[])
{
    Computer_Driver      computer(buildConfig(argc, argv));
    Pico_Driver          pico({});
    Camera_Driver        camera({});
    DiskChecker_Driver   disk({});
    Accelerometer_Driver acc1({ "/dev/i2c-1", 0x68 });
    Accelerometer_Driver acc2({ "/dev/i2c-1", 0x69 });
    runSession(computer, pico, camera, disk, acc1, acc2);
}

static void runInitialization(
    System &sys, Computer_Driver &computer, SendToComputer_UC &sender)
{

    while (!computer.isServerReachable())
    {
        sleep(5);
        printf("wait for server.\n");
    }
    printf("server has been reach.\n");
    sys.ready();
    sender.sendState(sys);
}

static void runLoop(
    System &sys, CommandReceptor_UC &receptor,
    ExecuteOrder_UC &executor, SendToComputer_UC &sender)
{
    while (true)
        runOneCommand(sys, receptor, executor, sender);
}

static void runOneCommand(
    System &sys, CommandReceptor_UC &receptor, ExecuteOrder_UC &executor, SendToComputer_UC &sender)
{
    if (!receptor.waitAndInterprete(sys))
        return;
    sys.processing();
    sender.sendState(sys);
    executor.execute(sys);
    sys.reset();
    sender.sendState(sys);
}
