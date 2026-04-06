#include "4_drivers/MotorDC_Driver/MotorDC_Driver.hpp"

#include <string>

namespace
{
enum class Dir
{
	STOP,
	UP,
	DOWN,
};

struct Harness
{
	Dir      dir = Dir::STOP;
	uint8_t  last_speed = 0;
	int      stop_calls = 0;
	uint32_t now = 0;
	uint32_t up_trigger_ms = 0;
	uint32_t down_trigger_ms = 0;
	bool     force_up_pressed = false;
	bool     force_down_pressed = false;

	MotorDC_DriverIO io()
	{
		return MotorDC_DriverIO{
			[this](uint8_t speed) {
				dir = Dir::UP;
				last_speed = speed;
			},
			[this](uint8_t speed) {
				dir = Dir::DOWN;
				last_speed = speed;
			},
			[this]() {
				dir = Dir::STOP;
				stop_calls++;
			},
			[this]() {
				if (force_up_pressed)
					return true;
				return dir == Dir::UP && now >= up_trigger_ms;
			},
			[this]() {
				if (force_down_pressed)
					return true;
				return dir == Dir::DOWN && now >= down_trigger_ms;
			},
			[this]() { return now; },
			[this](uint32_t ms) { now += ms; },
		};
	}
};
} // namespace

extern "C"
{
	#include "unity.h"

	void setUp(void) {}
	void tearDown(void) {}
}

void test_goInitialPos_already_up_returns_true_without_moving()
{
	Harness h;
	h.force_up_pressed = true;

	MotorDC_DriverConfig cfg;
	cfg.speed_percent = 70;
	cfg.timeout_ms = 200;
	cfg.poll_interval_ms = 5;

	MotorDC_Driver driver(cfg, h.io());
	TEST_ASSERT_TRUE(driver.goInitialPos());
	TEST_ASSERT_EQUAL_INT(Dir::STOP, static_cast<int>(h.dir));
	TEST_ASSERT_EQUAL_INT(1, h.stop_calls);
}

void test_goTo_down_reaches_button_before_timeout()
{
	Harness h;
	h.down_trigger_ms = 30;

	MotorDC_DriverConfig cfg;
	cfg.speed_percent = 55;
	cfg.timeout_ms = 200;
	cfg.poll_interval_ms = 5;

	MotorDC_Driver driver(cfg, h.io());
	TEST_ASSERT_TRUE(driver.goTo(Pos::DOWN));
	TEST_ASSERT_EQUAL_INT(Dir::STOP, static_cast<int>(h.dir));
	TEST_ASSERT_EQUAL_INT(55, h.last_speed);
	TEST_ASSERT_EQUAL_INT(1, h.stop_calls);
}

void test_goTo_up_times_out_and_returns_false()
{
	Harness h;
	h.up_trigger_ms = 9999;

	MotorDC_DriverConfig cfg;
	cfg.speed_percent = 40;
	cfg.timeout_ms = 20;
	cfg.poll_interval_ms = 5;

	MotorDC_Driver driver(cfg, h.io());
	TEST_ASSERT_FALSE(driver.goTo(Pos::UP));
	TEST_ASSERT_EQUAL_INT(Dir::STOP, static_cast<int>(h.dir));
	TEST_ASSERT_EQUAL_INT(1, h.stop_calls);
}

void test_goTo_unknown_returns_false()
{
	Harness h;

	MotorDC_DriverConfig cfg;
	cfg.speed_percent = 60;
	cfg.timeout_ms = 20;
	cfg.poll_interval_ms = 5;

	MotorDC_Driver driver(cfg, h.io());
	TEST_ASSERT_FALSE(driver.goTo(Pos::UNKNOWN));
	TEST_ASSERT_EQUAL_INT(Dir::STOP, static_cast<int>(h.dir));
	TEST_ASSERT_EQUAL_INT(0, h.stop_calls);
}

void test_speed_is_fixed_from_constructor()
{
	Harness h;
	h.up_trigger_ms = 10;
	h.down_trigger_ms = 10;

	MotorDC_DriverConfig cfg;
	cfg.speed_percent = 33;
	cfg.timeout_ms = 200;
	cfg.poll_interval_ms = 5;

	MotorDC_Driver driver(cfg, h.io());
	TEST_ASSERT_TRUE(driver.goTo(Pos::UP));
	TEST_ASSERT_EQUAL_INT(33, h.last_speed);
	TEST_ASSERT_TRUE(driver.goTo(Pos::DOWN));
	TEST_ASSERT_EQUAL_INT(33, h.last_speed);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_goInitialPos_already_up_returns_true_without_moving);
	RUN_TEST(test_goTo_down_reaches_button_before_timeout);
	RUN_TEST(test_goTo_up_times_out_and_returns_false);
	RUN_TEST(test_goTo_unknown_returns_false);
	RUN_TEST(test_speed_is_fixed_from_constructor);
	return UNITY_END();
}
