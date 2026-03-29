#include "1_domain/System.hpp"
#include <string>

extern "C"
{
	#include "unity.h"

	void setUp(void) {}
	void tearDown(void) {}
}

void hello_world(void)
 {
	TEST_ASSERT_TRUE(true);
 }


void test_system_workflow()
{
	System sys;
	TEST_ASSERT_EQUAL_INT(LISTENING, sys.get_state());

	sys.command_received();
	TEST_ASSERT_EQUAL_INT(INTERPRETING, sys.get_state());

	sys.command_interpreted(false);
	TEST_ASSERT_EQUAL_INT(LISTENING, sys.get_state());

	sys.command_interpreted(true);
	TEST_ASSERT_EQUAL_INT(EXECUTING, sys.get_state());

	TEST_ASSERT_EQUAL_INT(true, sys.result_command(0));

	TEST_ASSERT_EQUAL_INT(LISTENING, sys.get_state());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_system_workflow);
	return UNITY_END();
}
