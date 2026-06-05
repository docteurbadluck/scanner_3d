#include <stdio.h>
#include <pico/stdlib.h>

#include "0_orchestration/Drivers.hpp"
#include "0_orchestration/UseCases.hpp"
#include "0_orchestration/SelfTest.hpp"

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

static void halt_with_led()
{
    gpio_put(PICO_DEFAULT_LED_PIN, true);
    while (true) {}
}

static void init_hardware()
{
    stdio_init_all();
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, false);
    while (!stdio_usb_connected())
        sleep_ms(100);
    printf("[pico] Welcome on pico!\n");
}

int main()
{
    init_hardware();
    Drivers drv;
    if (!runSelfTests(drv))
        halt_with_led();
    printf("[pico] all Drivers test passed.\n");
    return 1;
    UseCases uc(drv);
    System   sys;
    run_loop(uc, sys);
    return 0;
}
