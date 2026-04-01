#include "3_interface/IMotorDC.hpp"
#include "3_interface/IServoMotor.hpp"
#include "3_interface/IStepperMotor.hpp"
#include "3_interface/IAccelerometer.hpp"
#include "3_interface/ISender.hpp"
#include "2_usecases/ArmController_UC/ArmController_UC.hpp"
#include "2_usecases/HandController_UC/HandController_UC.hpp"
#include "2_usecases/PlateController_UC/PlateController_UC.hpp"
#include "2_usecases/VibrationMonitor_UC/VibrationMonitor_UC.hpp"
#include "2_usecases/SendToPi_UC/SendToPi_UC.hpp"
#include "2_usecases/ExecuteCommand_UC/ExecuteCommand_UC.hpp"
#include "1_domain/System.hpp"
#include <string>

class mockMotorDC : public IMotorDC
{
public:
	bool _res = true;
	bool goInitialPos() override { return _res; }
	bool goTo(Pos pos) override { (void)pos; return _res; }
};

class mockServo : public IServoMotor
{
public:
	bool _res = true;
	bool goInitialPos() override { return _res; }
	bool goTo(Pos_hand pos) override { (void)pos; return _res; }
};

class mockStepper : public IStepperMotor
{
public:
	bool _res = true;
	bool goInitialPos() override { return _res; }
	bool stepForward() override { return _res; }
};

class mockAccelerometer : public IAccelerometer
{
public:
	float readMagnitude() override { return 0.0f; }
};

class mockSender : public ISender
{
public:
	std::string _lastMsg;
	void send(const std::string &msg) override { _lastMsg = msg; }
};

extern "C"
{
	#include "unity.h"

	void setUp(void) {}
	void tearDown(void) {}
}

static void run_cmd(ExecuteCommand_UC &exec, System &sys, const std::string &cmd)
{
	sys.setCommandToExecute(cmd);
	exec.execute(sys);
}

void test_execute_arm_up()
{
	mockMotorDC motor; mockServo servo; mockStepper stepper;
	mockAccelerometer acc1, acc2; mockSender sender;
	ArmController_UC   arm(motor);
	HandController_UC  hand(servo);
	PlateController_UC plate(stepper);
	VibrationMonitor_UC vib(acc1, acc2);
	SendToPi_UC sendUC(sender);
	ExecuteCommand_UC exec(arm, hand, plate, vib, sendUC);
	System sys;

	run_cmd(exec, sys, "ARM_UP");
	TEST_ASSERT_EQUAL_STRING("DONE", sender._lastMsg.c_str());
	TEST_ASSERT_EQUAL_INT(LISTENING, sys.getState());
}

void test_execute_arm_down()
{
	mockMotorDC motor; mockServo servo; mockStepper stepper;
	mockAccelerometer acc1, acc2; mockSender sender;
	ArmController_UC   arm(motor);
	HandController_UC  hand(servo);
	PlateController_UC plate(stepper);
	VibrationMonitor_UC vib(acc1, acc2);
	SendToPi_UC sendUC(sender);
	ExecuteCommand_UC exec(arm, hand, plate, vib, sendUC);
	System sys;

	arm.joinPos(Pos::UP);
	run_cmd(exec, sys, "ARM_DOWN");
	TEST_ASSERT_EQUAL_STRING("DONE", sender._lastMsg.c_str());
}

void test_execute_cam_a()
{
	mockMotorDC motor; mockServo servo; mockStepper stepper;
	mockAccelerometer acc1, acc2; mockSender sender;
	ArmController_UC   arm(motor);
	HandController_UC  hand(servo);
	PlateController_UC plate(stepper);
	VibrationMonitor_UC vib(acc1, acc2);
	SendToPi_UC sendUC(sender);
	ExecuteCommand_UC exec(arm, hand, plate, vib, sendUC);
	System sys;

	run_cmd(exec, sys, "CAM_A");
	TEST_ASSERT_EQUAL_STRING("DONE", sender._lastMsg.c_str());
}

void test_execute_cam_b()
{
	mockMotorDC motor; mockServo servo; mockStepper stepper;
	mockAccelerometer acc1, acc2; mockSender sender;
	ArmController_UC   arm(motor);
	HandController_UC  hand(servo);
	PlateController_UC plate(stepper);
	VibrationMonitor_UC vib(acc1, acc2);
	SendToPi_UC sendUC(sender);
	ExecuteCommand_UC exec(arm, hand, plate, vib, sendUC);
	System sys;

	hand.joinPos(Pos_hand::UP_A);
	run_cmd(exec, sys, "CAM_B");
	TEST_ASSERT_EQUAL_STRING("DONE", sender._lastMsg.c_str());
}

void test_execute_cam_c()
{
	mockMotorDC motor; mockServo servo; mockStepper stepper;
	mockAccelerometer acc1, acc2; mockSender sender;
	ArmController_UC   arm(motor);
	HandController_UC  hand(servo);
	PlateController_UC plate(stepper);
	VibrationMonitor_UC vib(acc1, acc2);
	SendToPi_UC sendUC(sender);
	ExecuteCommand_UC exec(arm, hand, plate, vib, sendUC);
	System sys;

	hand.joinPos(Pos_hand::UP_B);
	run_cmd(exec, sys, "CAM_C");
	TEST_ASSERT_EQUAL_STRING("DONE", sender._lastMsg.c_str());
}

void test_execute_cam_d()
{
	mockMotorDC motor; mockServo servo; mockStepper stepper;
	mockAccelerometer acc1, acc2; mockSender sender;
	ArmController_UC   arm(motor);
	HandController_UC  hand(servo);
	PlateController_UC plate(stepper);
	VibrationMonitor_UC vib(acc1, acc2);
	SendToPi_UC sendUC(sender);
	ExecuteCommand_UC exec(arm, hand, plate, vib, sendUC);
	System sys;

	hand.joinPos(Pos_hand::DOWN_A);
	run_cmd(exec, sys, "CAM_D");
	TEST_ASSERT_EQUAL_STRING("DONE", sender._lastMsg.c_str());
}

void test_execute_plate_next()
{
	mockMotorDC motor; mockServo servo; mockStepper stepper;
	mockAccelerometer acc1, acc2; mockSender sender;
	ArmController_UC   arm(motor);
	HandController_UC  hand(servo);
	PlateController_UC plate(stepper);
	VibrationMonitor_UC vib(acc1, acc2);
	SendToPi_UC sendUC(sender);
	ExecuteCommand_UC exec(arm, hand, plate, vib, sendUC);
	System sys;

	plate.joinInitialPos();
	run_cmd(exec, sys, "PLATE_NEXT");
	TEST_ASSERT_EQUAL_STRING("DONE", sender._lastMsg.c_str());
}

void test_execute_initial_pos()
{
	mockMotorDC motor; mockServo servo; mockStepper stepper;
	mockAccelerometer acc1, acc2; mockSender sender;
	ArmController_UC   arm(motor);
	HandController_UC  hand(servo);
	PlateController_UC plate(stepper);
	VibrationMonitor_UC vib(acc1, acc2);
	SendToPi_UC sendUC(sender);
	ExecuteCommand_UC exec(arm, hand, plate, vib, sendUC);
	System sys;

	run_cmd(exec, sys, "INITIAL_POS");
	TEST_ASSERT_EQUAL_STRING("DONE", sender._lastMsg.c_str());
}

void test_execute_initial_pos_fail()
{
	mockMotorDC motor; mockServo servo; mockStepper stepper;
	mockAccelerometer acc1, acc2; mockSender sender;
	motor._res = false;
	ArmController_UC   arm(motor);
	HandController_UC  hand(servo);
	PlateController_UC plate(stepper);
	VibrationMonitor_UC vib(acc1, acc2);
	SendToPi_UC sendUC(sender);
	ExecuteCommand_UC exec(arm, hand, plate, vib, sendUC);
	System sys;

	run_cmd(exec, sys, "INITIAL_POS");
	TEST_ASSERT_EQUAL_STRING("FAIL", sender._lastMsg.c_str());
}

void test_execute_ping()
{
	mockMotorDC motor; mockServo servo; mockStepper stepper;
	mockAccelerometer acc1, acc2; mockSender sender;
	ArmController_UC   arm(motor);
	HandController_UC  hand(servo);
	PlateController_UC plate(stepper);
	VibrationMonitor_UC vib(acc1, acc2);
	SendToPi_UC sendUC(sender);
	ExecuteCommand_UC exec(arm, hand, plate, vib, sendUC);
	System sys;

	run_cmd(exec, sys, "PING");
	TEST_ASSERT_EQUAL_STRING("PONG", sender._lastMsg.c_str());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_execute_arm_up);
	RUN_TEST(test_execute_arm_down);
	RUN_TEST(test_execute_cam_a);
	RUN_TEST(test_execute_cam_b);
	RUN_TEST(test_execute_cam_c);
	RUN_TEST(test_execute_cam_d);
	RUN_TEST(test_execute_plate_next);
	RUN_TEST(test_execute_initial_pos);
	RUN_TEST(test_execute_initial_pos_fail);
	RUN_TEST(test_execute_ping);
	return UNITY_END();
}
