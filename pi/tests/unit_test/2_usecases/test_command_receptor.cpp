#include "3_interface/IComputer.hpp"
#include "3_interface/ISender.hpp"
#include "2_usecases/SendToComputer_UC/SendToComputer_UC.hpp"
#include "2_usecases/CommandReceptor_UC/CommandReceptor_UC.hpp"
#include "1_domain/System/System.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"
#include <string>

class mockHttpUploader : public IComputer
{
public:
	bool        _res;
	std::string _order;
	bool        isServerReachable() override { return _res; }
	bool        upload(const std::string &) override { return _res; }
	std::string waitForOrder() override { return _order; }
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

	void setUp(void)    {}
	void tearDown(void) {}
}

void test_CommandReceptor_valid_command()
{
	mockHttpUploader uploader;
	mockSender       sender;
	uploader._order = JsonMessage::makeCommand("TAKE_PHOTO");
	SendToComputer_UC   sendUC(sender);
	CommandReceptor_UC  uc(uploader, sendUC);
	System sys;

	bool res = uc.waitAndInterprete(sys);
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_STRING("TAKE_PHOTO", sys.getCommandToExecute().c_str());
	TEST_ASSERT_EQUAL_STRING("TAKE_PHOTO", uc.getMessage().c_str());
}

void test_CommandReceptor_invalid_command()
{
	mockHttpUploader uploader;
	mockSender       sender;
	uploader._order = JsonMessage::makeCommand("UNKNOWN");
	SendToComputer_UC   sendUC(sender);
	CommandReceptor_UC  uc(uploader, sendUC);
	System sys;

	bool res = uc.waitAndInterprete(sys);
	TEST_ASSERT_FALSE(res);
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeError("INVALID_CMD").c_str(), sender._lastMsg.c_str());
}

void test_CommandReceptor_sends_state_on_valid()
{
	mockHttpUploader uploader;
	mockSender       sender;
	uploader._order = JsonMessage::makeCommand("PING");
	SendToComputer_UC   sendUC(sender);
	CommandReceptor_UC  uc(uploader, sendUC);
	System sys;
	sys.ready();

	uc.waitAndInterprete(sys);
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeState("READY").c_str(), sender._lastMsg.c_str());
}

void test_CommandReceptor_all_valid_commands()
{
	const std::string cmds[] = {"PING", "PONG", "GET_STATUS", "GET_PICO_STATUS", "START_CAPTURE", "TAKE_PHOTO"};
	for (const std::string &cmd : cmds)
	{
		mockHttpUploader uploader;
		mockSender       sender;
		uploader._order = JsonMessage::makeCommand(cmd);
		SendToComputer_UC   sendUC(sender);
		CommandReceptor_UC  uc(uploader, sendUC);
		System sys;

		TEST_ASSERT_TRUE(uc.waitAndInterprete(sys));
		TEST_ASSERT_EQUAL_STRING(cmd.c_str(), sys.getCommandToExecute().c_str());
	}
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_CommandReceptor_valid_command);
	RUN_TEST(test_CommandReceptor_invalid_command);
	RUN_TEST(test_CommandReceptor_sends_state_on_valid);
	RUN_TEST(test_CommandReceptor_all_valid_commands);
	return UNITY_END();
}
