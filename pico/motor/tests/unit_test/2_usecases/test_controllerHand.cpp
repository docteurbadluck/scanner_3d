#include "3_interface/IServoMotor.hpp"
#include "2_usecases/HandController_UC/HandController_UC.hpp"

class mockServoMotor : public IServoMotor
{
public:
	bool _res;
	bool goInitialPos() override { return _res; }
	bool goTo(Pos_hand pos) override { (void)pos; return _res; }
};

extern "C"
{
	#include "unity.h"

	void setUp(void) {}
	void tearDown(void) {}
}

void test_HandController_UC_success()
{
	mockServoMotor mockServo;
	mockServo._res = true;
	HandController_UC controller(mockServo);
	TEST_ASSERT_EQUAL_INT((int)Pos_hand::UNKNOWN, (int)controller.getPos());

	bool res = controller.joinInitialPos();
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_INT((int)Pos_hand::UP_A, (int)controller.getPos());

	res = controller.joinPos(Pos_hand::DOWN_A);
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_INT((int)Pos_hand::DOWN_A, (int)controller.getPos());

	res = controller.joinPos(Pos_hand::DOWN_A);
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_INT((int)Pos_hand::DOWN_A, (int)controller.getPos());
}

void test_HandController_UC_fail()
{
	mockServoMotor mockServo;
	mockServo._res = false;
	HandController_UC controller(mockServo);
	TEST_ASSERT_EQUAL_INT((int)Pos_hand::UNKNOWN, (int)controller.getPos());

	bool res = controller.joinInitialPos();
	TEST_ASSERT_FALSE(res);
	TEST_ASSERT_EQUAL_INT((int)Pos_hand::UNKNOWN, (int)controller.getPos());

	mockServo._res = true;
	controller.joinPos(Pos_hand::UP_B);
	TEST_ASSERT_EQUAL_INT((int)Pos_hand::UP_B, (int)controller.getPos());
	mockServo._res = false;

	controller.joinPos(Pos_hand::DOWN_B);
	TEST_ASSERT_EQUAL_INT((int)Pos_hand::UNKNOWN, (int)controller.getPos());
}

void test_HandController_UC_all_positions()
{
	mockServoMotor mockServo;
	mockServo._res = true;
	HandController_UC controller(mockServo);

	controller.joinPos(Pos_hand::UP_A);
	TEST_ASSERT_EQUAL_INT((int)Pos_hand::UP_A, (int)controller.getPos());

	controller.joinPos(Pos_hand::UP_B);
	TEST_ASSERT_EQUAL_INT((int)Pos_hand::UP_B, (int)controller.getPos());

	controller.joinPos(Pos_hand::DOWN_A);
	TEST_ASSERT_EQUAL_INT((int)Pos_hand::DOWN_A, (int)controller.getPos());

	controller.joinPos(Pos_hand::DOWN_B);
	TEST_ASSERT_EQUAL_INT((int)Pos_hand::DOWN_B, (int)controller.getPos());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_HandController_UC_success);
	RUN_TEST(test_HandController_UC_fail);
	RUN_TEST(test_HandController_UC_all_positions);
	return UNITY_END();
}
