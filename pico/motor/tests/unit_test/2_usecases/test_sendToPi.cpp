#include "3_interface/ISender.hpp"
#include "2_usecases/SendToPi_UC/SendToPi_UC.hpp"
#include "1_domain/System/System.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"
#include <string>

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

void test_sendState_listening()
{
	mockSender sender;
	SendToPi_UC uc(sender);
	System sys;
	uc.sendState(sys);
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeState("LISTENING").c_str(), sender._lastMsg.c_str());
}

void test_sendState_interpreting()
{
	mockSender sender;
	SendToPi_UC uc(sender);
	System sys;
	sys.commandReceived();
	uc.sendState(sys);
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeState("INTERPRETING").c_str(), sender._lastMsg.c_str());
}

void test_sendState_executing()
{
	mockSender sender;
	SendToPi_UC uc(sender);
	System sys;
	sys.commandReceived();
	sys.commandInterpreted(true);
	uc.sendState(sys);
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeState("EXECUTING").c_str(), sender._lastMsg.c_str());
}

void test_sendInvalidCmd()
{
	mockSender sender;
	SendToPi_UC uc(sender);
	uc.sendInvalidCmd();
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeError("INVALID_CMD").c_str(), sender._lastMsg.c_str());
}

void test_sendResponse_ping()
{
	mockSender sender;
	SendToPi_UC uc(sender);
	System sys;
	uc.sendResponse("PING", true, sys);
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("PONG", "PING").c_str(), sender._lastMsg.c_str());
}

void test_sendResponse_get_status()
{
	mockSender sender;
	SendToPi_UC uc(sender);
	System sys;
	uc.sendResponse("GET_STATUS", true, sys);
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeState("LISTENING").c_str(), sender._lastMsg.c_str());
}

void test_sendResponse_done()
{
	mockSender sender;
	SendToPi_UC uc(sender);
	System sys;
	uc.sendResponse("ARM_UP", true, sys);
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("DONE", "ARM_UP").c_str(), sender._lastMsg.c_str());
}

void test_sendResponse_fail()
{
	mockSender sender;
	SendToPi_UC uc(sender);
	System sys;
	uc.sendResponse("ARM_UP", false, sys);
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("FAIL", "ARM_UP").c_str(), sender._lastMsg.c_str());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_sendState_listening);
	RUN_TEST(test_sendState_interpreting);
	RUN_TEST(test_sendState_executing);
	RUN_TEST(test_sendInvalidCmd);
	RUN_TEST(test_sendResponse_ping);
	RUN_TEST(test_sendResponse_get_status);
	RUN_TEST(test_sendResponse_done);
	RUN_TEST(test_sendResponse_fail);
	return UNITY_END();
}
