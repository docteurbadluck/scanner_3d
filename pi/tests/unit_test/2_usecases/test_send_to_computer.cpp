#include "3_interface/ISender.hpp"
#include "2_usecases/SendToComputer_UC/SendToComputer_UC.hpp"
#include "1_domain/System.hpp"
#include "1_domain/PicoJson.hpp"
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

	void setUp(void)    {}
	void tearDown(void) {}
}

void test_sendState_initialization()
{
	mockSender sender;
	SendToComputer_UC uc(sender);
	System sys;
	uc.sendState(sys);
	TEST_ASSERT_EQUAL_STRING(PicoJson::makeState("INITIALIZATION").c_str(), sender._lastMsg.c_str());
}

void test_sendState_ready()
{
	mockSender sender;
	SendToComputer_UC uc(sender);
	System sys;
	sys.ready();
	uc.sendState(sys);
	TEST_ASSERT_EQUAL_STRING(PicoJson::makeState("READY").c_str(), sender._lastMsg.c_str());
}

void test_sendInvalidCmd()
{
	mockSender sender;
	SendToComputer_UC uc(sender);
	uc.sendInvalidCmd();
	TEST_ASSERT_EQUAL_STRING(PicoJson::makeError("INVALID_CMD").c_str(), sender._lastMsg.c_str());
}

void test_sendResponse_ping()
{
	mockSender sender;
	SendToComputer_UC uc(sender);
	System sys;
	uc.sendResponse("PING", true, sys);
	TEST_ASSERT_EQUAL_STRING(PicoJson::makeResponse("PONG", "PING").c_str(), sender._lastMsg.c_str());
}

void test_sendResponse_get_status()
{
	mockSender sender;
	SendToComputer_UC uc(sender);
	System sys;
	sys.ready();
	uc.sendResponse("GET_STATUS", true, sys);
	TEST_ASSERT_EQUAL_STRING(PicoJson::makeState("READY").c_str(), sender._lastMsg.c_str());
}

void test_sendResponse_done()
{
	mockSender sender;
	SendToComputer_UC uc(sender);
	System sys;
	uc.sendResponse("START_CAPTURE", true, sys);
	TEST_ASSERT_EQUAL_STRING(PicoJson::makeResponse("DONE", "START_CAPTURE").c_str(), sender._lastMsg.c_str());
}

void test_sendResponse_fail()
{
	mockSender sender;
	SendToComputer_UC uc(sender);
	System sys;
	uc.sendResponse("START_CAPTURE", false, sys);
	TEST_ASSERT_EQUAL_STRING(PicoJson::makeResponse("FAIL", "START_CAPTURE").c_str(), sender._lastMsg.c_str());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_sendState_initialization);
	RUN_TEST(test_sendState_ready);
	RUN_TEST(test_sendInvalidCmd);
	RUN_TEST(test_sendResponse_ping);
	RUN_TEST(test_sendResponse_get_status);
	RUN_TEST(test_sendResponse_done);
	RUN_TEST(test_sendResponse_fail);
	return UNITY_END();
}
