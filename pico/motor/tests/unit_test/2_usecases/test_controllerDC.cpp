#include "1_domain/System.hpp"
#include "2_usecases/ArmController_UC/ArmController_UC.hpp"
#include "3_interface/IMotorDC.hpp"
#include <string>


class mockMotorDC : public IMotorDC
{
	public :
		mockMotorDC(){};
		~mockMotorDC() = default;
		bool	_res;	
		bool	go_initial_pos() override {return _res;};
		bool	go_to(Pos position) override {(void) position; return _res;}; 
};

extern "C"
{
	#include "unity.h"

	void setUp(void) {}
	void tearDown(void) {}
}


void test_ArmController_UC_success()
{
	System sys;
	mockMotorDC mockMotor;
	mockMotor._res = true;
	ArmController_UC controllerDC(mockMotor);
	TEST_ASSERT_EQUAL_INT(UNKNOWN, controllerDC.get_pos());

	bool res = controllerDC.join_initial_pos();
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_INT(UP, controllerDC.get_pos());

	res = controllerDC.join_pos(DOWN);
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_INT(DOWN, controllerDC.get_pos());
}

void test_ArmController_UC_fail()
{
	System sys;
	mockMotorDC mockMotor;
	mockMotor._res = false;
	ArmController_UC controllerDC(mockMotor);
	TEST_ASSERT_EQUAL_INT(UNKNOWN, controllerDC.get_pos());

	bool res = controllerDC.join_initial_pos();
	TEST_ASSERT_FALSE(res);
	TEST_ASSERT_EQUAL_INT(UNKNOWN, controllerDC.get_pos());

	mockMotor._res = true;
	controllerDC.join_pos(DOWN);
	TEST_ASSERT_EQUAL_INT(DOWN, controllerDC.get_pos());
	mockMotor._res = false;
	
	controllerDC.join_pos(UP);
	TEST_ASSERT_EQUAL_INT(UNKNOWN, controllerDC.get_pos());
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_ArmController_UC_success);
	RUN_TEST(test_ArmController_UC_fail);

	return UNITY_END();
}
