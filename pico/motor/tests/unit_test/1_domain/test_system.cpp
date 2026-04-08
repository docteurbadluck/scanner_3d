#include "1_domain/System/System.hpp"
#include <string>

extern "C"
{
	#include "unity.h"

	void setUp(void) {}
	void tearDown(void) {}
}

void test_system_workflow()
{
	System sys;
	TEST_ASSERT_EQUAL_INT(LISTENING, sys.getState());

	sys.commandReceived();
	TEST_ASSERT_EQUAL_INT(INTERPRETING, sys.getState());

	sys.commandInterpreted(false);
	TEST_ASSERT_EQUAL_INT(LISTENING, sys.getState());

	sys.commandInterpreted(true);
	TEST_ASSERT_EQUAL_INT(EXECUTING, sys.getState());

	TEST_ASSERT_EQUAL_INT(true, sys.resultCommand(0));
	TEST_ASSERT_EQUAL_INT(LISTENING, sys.getState());
}

void test_system_resultCommand_fail()
{
	System sys;
	sys.commandReceived();
	sys.commandInterpreted(true);
	TEST_ASSERT_EQUAL_INT(false, sys.resultCommand(1));
	TEST_ASSERT_EQUAL_INT(LISTENING, sys.getState());
}

void test_system_getStateString()
{
	System sys;
	TEST_ASSERT_EQUAL_STRING("LISTENING", sys.getStateString().c_str());

	sys.commandReceived();
	TEST_ASSERT_EQUAL_STRING("INTERPRETING", sys.getStateString().c_str());

	sys.commandInterpreted(true);
	TEST_ASSERT_EQUAL_STRING("EXECUTING", sys.getStateString().c_str());
}

void test_system_command_to_execute()
{
	System sys;
	sys.setCommandToExecute("ARM_UP");
	TEST_ASSERT_EQUAL_STRING("ARM_UP", sys.getCommandToExecute().c_str());
}

void test_system_getCommands()
{
	System sys;
	Commands cmds = sys.getCommands();
	TEST_ASSERT_TRUE(cmds.valid_command.size() > 0);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_system_workflow);
	RUN_TEST(test_system_resultCommand_fail);
	RUN_TEST(test_system_getStateString);
	RUN_TEST(test_system_command_to_execute);
	RUN_TEST(test_system_getCommands);
	return UNITY_END();
}
