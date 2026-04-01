#include "1_domain/System.hpp"
#include "2_usecases/ArmController_UC/ArmController_UC.hpp"
#include "3_interface/IMotorDC.hpp"
#include <string>


class mockMotorDC : public IMotorDC
{
public:
	mockMotorDC() {};
	~mockMotorDC() = default;
	bool _res;
	bool goInitialPos() override { return _res; }
	bool goTo(Pos position) override { (void)position; return _res; }
};

extern "C"
{
	#include "unity.h"

	void setUp(void) {}
	void tearDown(void) {}
}


void test_ArmController_UC_success()
{
	mockMotorDC mockMotor;
	mockMotor._res = true;
	ArmController_UC controllerDC(mockMotor);
	TEST_ASSERT_EQUAL_INT((int)Pos::UNKNOWN, (int)controllerDC.getPos());

	bool res = controllerDC.joinInitialPos();
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_INT((int)Pos::UP, (int)controllerDC.getPos());

	res = controllerDC.joinPos(Pos::DOWN);
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_INT((int)Pos::DOWN, (int)controllerDC.getPos());

	res = controllerDC.joinPos(Pos::DOWN);
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_INT((int)Pos::DOWN, (int)controllerDC.getPos());
}

void test_ArmController_UC_fail()
{
	mockMotorDC mockMotor;
	mockMotor._res = false;
	ArmController_UC controllerDC(mockMotor);
	TEST_ASSERT_EQUAL_INT((int)Pos::UNKNOWN, (int)controllerDC.getPos());

	bool res = controllerDC.joinInitialPos();
	TEST_ASSERT_FALSE(res);
	TEST_ASSERT_EQUAL_INT((int)Pos::UNKNOWN, (int)controllerDC.getPos());

	mockMotor._res = true;
	controllerDC.joinPos(Pos::DOWN);
	TEST_ASSERT_EQUAL_INT((int)Pos::DOWN, (int)controllerDC.getPos());
	mockMotor._res = false;

	controllerDC.joinPos(Pos::UP);
	TEST_ASSERT_EQUAL_INT((int)Pos::UNKNOWN, (int)controllerDC.getPos());
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_ArmController_UC_success);
	RUN_TEST(test_ArmController_UC_fail);

	return UNITY_END();
}
