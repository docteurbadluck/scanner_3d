#include "2_usecases/PlateController_UC/PlateController_UC.hpp"
#include "3_interface/IStepperMotor.hpp"

class mockStepper : public IStepperMotor
{
	public:
		mockStepper() {};
		~mockStepper() = default;
		bool	_res;
		bool	go_initial_pos() override { return _res; }
		bool	step_forward()   override { return _res; }
};

extern "C"
{
	#include "unity.h"

	void setUp(void)    {}
	void tearDown(void) {}
}

void test_PlateController_UC_initial_pos_success()
{
	mockStepper mock;
	mock._res = true;
	PlateController_UC ctrl(mock);
	TEST_ASSERT_EQUAL_INT(-1, ctrl.get_pos());

	bool res = ctrl.join_initial_pos();
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_INT(0, ctrl.get_pos());
}

void test_PlateController_UC_initial_pos_fail()
{
	mockStepper mock;
	mock._res = false;
	PlateController_UC ctrl(mock);

	bool res = ctrl.join_initial_pos();
	TEST_ASSERT_FALSE(res);
	TEST_ASSERT_EQUAL_INT(-1, ctrl.get_pos());
}

void test_PlateController_UC_rotate_to_success()
{
	mockStepper mock;
	mock._res = true;
	PlateController_UC ctrl(mock);
	ctrl.join_initial_pos();

	bool res = ctrl.rotate_to(3);
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_INT(3, ctrl.get_pos());
}

void test_PlateController_UC_rotate_to_same_pos()
{
	mockStepper mock;
	mock._res = true;
	PlateController_UC ctrl(mock);
	ctrl.join_initial_pos();

	bool res = ctrl.rotate_to(0);
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_INT(0, ctrl.get_pos());
}

void test_PlateController_UC_rotate_wraps_around()
{
	mockStepper mock;
	mock._res = true;
	PlateController_UC ctrl(mock);
	ctrl.join_initial_pos();

	ctrl.rotate_to(35);
	bool res = ctrl.rotate_to(2);
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_INT(2, ctrl.get_pos());
}

void test_PlateController_UC_rotate_fails_mid_way()
{
	mockStepper mock;
	mock._res = true;
	PlateController_UC ctrl(mock);
	ctrl.join_initial_pos();

	mock._res = false;
	bool res = ctrl.rotate_to(5);
	TEST_ASSERT_FALSE(res);
	TEST_ASSERT_EQUAL_INT(-1, ctrl.get_pos());
}

void test_PlateController_UC_rotate_before_init()
{
	mockStepper mock;
	mock._res = true;
	PlateController_UC ctrl(mock);

	bool res = ctrl.rotate_to(3);
	TEST_ASSERT_FALSE(res);
	TEST_ASSERT_EQUAL_INT(-1, ctrl.get_pos());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_PlateController_UC_initial_pos_success);
	RUN_TEST(test_PlateController_UC_initial_pos_fail);
	RUN_TEST(test_PlateController_UC_rotate_to_success);
	RUN_TEST(test_PlateController_UC_rotate_to_same_pos);
	RUN_TEST(test_PlateController_UC_rotate_wraps_around);
	RUN_TEST(test_PlateController_UC_rotate_fails_mid_way);
	RUN_TEST(test_PlateController_UC_rotate_before_init);
	return UNITY_END();
}
