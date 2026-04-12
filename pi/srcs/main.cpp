#include "1_domain/System/System.hpp"
#include "2_usecases/CaptureData_UC/CaptureData_UC.hpp"
#include "2_usecases/CommandReceptor_UC/CommandReceptor_UC.hpp"
#include "2_usecases/ExecuteOrder_UC/ExecuteOrder_UC.hpp"
#include "2_usecases/SendPhotoToComputer_UC/SendPhotoToComputer_UC.hpp"
#include "2_usecases/SendToComputer_UC/SendToComputer_UC.hpp"
#include "4_drivers/Camera_Driver/Camera_Driver.hpp"
#include "4_drivers/Computer_Driver/Computer_Driver.hpp"
#include "4_drivers/DiskChecker_Driver/DiskChecker_Driver.hpp"
#include "4_drivers/Pico_Driver/Pico_Driver.hpp"
#include <unistd.h>

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

static Computer_DriverConfig buildConfig(int argc, char *argv[])
{
    Computer_DriverConfig cfg;
    cfg.host = (argc >= 2) ? argv[1] : "127.0.0.1";
    return cfg;
}

static void runLoop(
    System &sys, CommandReceptor_UC &receptor,
    ExecuteOrder_UC &executor, SendToComputer_UC &sender)
{
    while (true)
        runOneCommand(sys, receptor, executor, sender);
}

int main(int argc, char *argv[])
{
    Computer_Driver    computer(buildConfig(argc, argv));
    Pico_Driver        pico({});
    Camera_Driver      camera({});
    DiskChecker_Driver disk({});
    SendToComputer_UC      sender(computer);
    CaptureData_UC         capture(pico, camera, disk);
    SendPhotoToComputer_UC sendPhoto(camera, computer);
    CommandReceptor_UC     receptor(computer, sender);
    ExecuteOrder_UC        executor(capture, sendPhoto, sender, pico);
    System sys;
    runInitialization(sys, computer, sender);
    runLoop(sys, receptor, executor, sender);
}
