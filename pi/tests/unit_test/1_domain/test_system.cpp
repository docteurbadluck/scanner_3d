#include "1_domain/System.hpp"

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

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_System_initial_state);
	RUN_TEST(test_System_ready);
	RUN_TEST(test_System_processing);
	RUN_TEST(test_System_sending);
	RUN_TEST(test_System_reset);
	return UNITY_END();
}
