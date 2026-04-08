#include "1_domain/System/System.hpp"

extern "C"
{
	#include "unity.h"

	void setUp(void)    {}
	void tearDown(void) {}
}

void test_System_initial_state()
{
	System sys;
	TEST_ASSERT_EQUAL_INT(INITIALIZATION, sys.get_state());
}

void test_System_ready()
{
	System sys;
	sys.ready();
	TEST_ASSERT_EQUAL_INT(READY, sys.get_state());
}

void test_System_processing()
{
	System sys;
	sys.ready();
	sys.processing();
	TEST_ASSERT_EQUAL_INT(PROCESSING, sys.get_state());
}

void test_System_sending()
{
	System sys;
	sys.ready();
	sys.processing();
	sys.sending();
	TEST_ASSERT_EQUAL_INT(SENDING, sys.get_state());
}

void test_System_reset()
{
	System sys;
	sys.ready();
	sys.processing();
	sys.reset();
	TEST_ASSERT_EQUAL_INT(INITIALIZATION, sys.get_state());
}

void test_System_getStateString()
{
	System sys;
	TEST_ASSERT_EQUAL_STRING("INITIALIZATION", sys.getStateString().c_str());
	sys.ready();
	TEST_ASSERT_EQUAL_STRING("READY", sys.getStateString().c_str());
	sys.processing();
	TEST_ASSERT_EQUAL_STRING("PROCESSING", sys.getStateString().c_str());
	sys.sending();
	TEST_ASSERT_EQUAL_STRING("SENDING", sys.getStateString().c_str());
}

void test_System_command_to_execute()
{
	System sys;
	sys.setCommandToExecute("START_CAPTURE");
	TEST_ASSERT_EQUAL_STRING("START_CAPTURE", sys.getCommandToExecute().c_str());
}

void test_System_getCommands()
{
	System sys;
	Commands cmds = sys.getCommands();
	TEST_ASSERT_TRUE(cmds.valid_command.size() > 0);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_System_initial_state);
	RUN_TEST(test_System_ready);
	RUN_TEST(test_System_processing);
	RUN_TEST(test_System_sending);
	RUN_TEST(test_System_reset);
	RUN_TEST(test_System_getStateString);
	RUN_TEST(test_System_command_to_execute);
	RUN_TEST(test_System_getCommands);
	return UNITY_END();
}
