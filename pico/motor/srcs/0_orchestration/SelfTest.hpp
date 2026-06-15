#pragma once

#include "0_orchestration/Drivers.hpp"

typedef struct s_test_result
{
	bool servo_is_ok;
	bool dc_is_ok;
	bool steeper_is_ok;
	bool accel_arm_is_ok;
	bool accel_plate_is_ok;
	bool are_tests_failed;
} t_test_result;

[[nodiscard]] t_test_result runSelfTests(Drivers &drv);


