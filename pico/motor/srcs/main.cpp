#include <stdio.h>
#include <pico/stdlib.h>
#include "0_orchestration/Drivers.hpp"
#include "0_orchestration/UseCases.hpp"

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

static void init_hardware()
{
    stdio_init_all();
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, false);
}

int main()
{
    init_hardware();
    Drivers drv;
    UseCases uc(drv);
    System   sys;
    run_loop(uc, sys);
    return 0;
}
