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

static bool test_stepper(Drivers &drv)
{
    drv.stepper.selfTest();
    return true;
}

t_test_result runSelfTests(Drivers &drv)
{
    t_test_result results;

    results.dc_is_ok = test_motor_dc(drv);
    results.stepper_is_ok = test_stepper(drv);
    results.servo_is_ok = test_servo(drv);
	results.are_tests_failed = !(results.dc_is_ok &&
			results.stepper_is_ok &&
			results.servo_is_ok);
	return results;
}
