#include "2_usecases/VibrationMonitor_UC/VibrationMonitor_UC.hpp"
#include "3_interface/IAccelerometer.hpp"

class mockAccelerometer : public IAccelerometer
{
	public:
		mockAccelerometer() {};
		~mockAccelerometer() = default;
		float	_magnitude;
		float	read_magnitude() override { return _magnitude; }
};

extern "C"
{
	#include "unity.h"

	void setUp(void)    {}
	void tearDown(void) {}
}

void test_isStable_both_stable()
{
	mockAccelerometer acc1, acc2;
	acc1._magnitude = 0.01f;
	acc2._magnitude = 0.01f;
	VibrationMonitor_UC monitor(acc1, acc2);
	TEST_ASSERT_TRUE(monitor.isStable());
}

void test_isStable_first_unstable()
{
	mockAccelerometer acc1, acc2;
	acc1._magnitude = 0.10f;
	acc2._magnitude = 0.01f;
	VibrationMonitor_UC monitor(acc1, acc2);
	TEST_ASSERT_FALSE(monitor.isStable());
}

void test_isStable_second_unstable()
{
	mockAccelerometer acc1, acc2;
	acc1._magnitude = 0.01f;
	acc2._magnitude = 0.10f;
	VibrationMonitor_UC monitor(acc1, acc2);
	TEST_ASSERT_FALSE(monitor.isStable());
}

void test_isStable_both_unstable()
{
	mockAccelerometer acc1, acc2;
	acc1._magnitude = 0.10f;
	acc2._magnitude = 0.10f;
	VibrationMonitor_UC monitor(acc1, acc2);
	TEST_ASSERT_FALSE(monitor.isStable());
}

void test_isStable_at_threshold()
{
	mockAccelerometer acc1, acc2;
	acc1._magnitude = 0.05f;
	acc2._magnitude = 0.01f;
	VibrationMonitor_UC monitor(acc1, acc2);
	TEST_ASSERT_FALSE(monitor.isStable());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_isStable_both_stable);
	RUN_TEST(test_isStable_first_unstable);
	RUN_TEST(test_isStable_second_unstable);
	RUN_TEST(test_isStable_both_unstable);
	RUN_TEST(test_isStable_at_threshold);
	return UNITY_END();
}
