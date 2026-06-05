#include "SelfTest.hpp"

#include <stdio.h>

static bool test_motor_dc(Drivers &drv)
{
    const SelfTestResult res = drv.motorDC.selfTest();
    if (res == SelfTestResult::OK)
    {
        return true;
    }
    const char *names[] = { "OK", "DOWN_UNREACHABLE", "UP_UNREACHABLE" };
    printf("[selfTest] motorDC FAIL: %d (%s)\n", static_cast<int>(res), names[static_cast<int>(res)]);
    return false;
}

static bool test_servo(Drivers &drv)
{
    const ServoSelfTestResult res = drv.servo.selfTest();
    if (res == ServoSelfTestResult::OK)
    {
        return true;
    }
    printf("[selfTest] servo FAIL: NO_CURRENT\n");
    return false;
}

bool runSelfTests(Drivers &drv)
{
    bool ok = true;
    ok &= test_motor_dc(drv);
    ok &= test_servo(drv);
    return ok;
}
