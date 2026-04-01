#include "3_interface/IReceptor.hpp"
#include "3_interface/ISender.hpp"
#include "2_usecases/CommandReceptorUC/CommandReceptorUC.hpp"
#include "2_usecases/SendToPi_UC/SendToPi_UC.hpp"
#include "1_domain/System.hpp"
#include <string>

class mockReceptor : public IReceptor
{
public:
	bool        _arrived;
	std::string _msg;
	bool        isMessageArrived() override { return _arrived; }
	std::string getMessage() const override { return _msg; }
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

void test_checkForNewMessage_arrives()
{
	mockReceptor receptor;
	mockSender   sender;
	SendToPi_UC  sendUC(sender);
	System       sys;
	Commands     cmds = sys.getCommands();

	receptor._arrived = true;
	receptor._msg     = "ARM_UP";
	CommandReceptorUC uc(receptor, cmds, sendUC);

	uc.checkForNewMessage(sys);
	TEST_ASSERT_EQUAL_INT(INTERPRETING, sys.getState());
	TEST_ASSERT_EQUAL_STRING("INTERPRETING", sender._lastMsg.c_str());
}

void test_checkForNewMessage_no_message()
{
	mockReceptor receptor;
	mockSender   sender;
	SendToPi_UC  sendUC(sender);
	System       sys;
	Commands     cmds = sys.getCommands();

	receptor._arrived = false;
	CommandReceptorUC uc(receptor, cmds, sendUC);

	uc.checkForNewMessage(sys);
	TEST_ASSERT_EQUAL_INT(LISTENING, sys.getState());
}

void test_checkForNewMessage_not_listening()
{
	mockReceptor receptor;
	mockSender   sender;
	SendToPi_UC  sendUC(sender);
	System       sys;
	Commands     cmds = sys.getCommands();

	sys.commandReceived();
	receptor._arrived = true;
	receptor._msg     = "ARM_UP";
	CommandReceptorUC uc(receptor, cmds, sendUC);

	uc.checkForNewMessage(sys);
	TEST_ASSERT_EQUAL_INT(INTERPRETING, sys.getState());
}

void test_interpreteMessage_valid()
{
	mockReceptor receptor;
	mockSender   sender;
	SendToPi_UC  sendUC(sender);
	System       sys;
	Commands     cmds = sys.getCommands();

	receptor._arrived = true;
	receptor._msg     = "ARM_UP";
	CommandReceptorUC uc(receptor, cmds, sendUC);

	uc.checkForNewMessage(sys);
	bool res = uc.interpreteMessage(sys);
	TEST_ASSERT_TRUE(res);
	TEST_ASSERT_EQUAL_INT(EXECUTING, sys.getState());
	TEST_ASSERT_EQUAL_STRING("ARM_UP", sys.getCommandToExecute().c_str());
	TEST_ASSERT_EQUAL_STRING("EXECUTING", sender._lastMsg.c_str());
}

void test_interpreteMessage_invalid()
{
	mockReceptor receptor;
	mockSender   sender;
	SendToPi_UC  sendUC(sender);
	System       sys;
	Commands     cmds = sys.getCommands();

	receptor._arrived = true;
	receptor._msg     = "UNKNOWN_CMD";
	CommandReceptorUC uc(receptor, cmds, sendUC);

	uc.checkForNewMessage(sys);
	bool res = uc.interpreteMessage(sys);
	TEST_ASSERT_FALSE(res);
	TEST_ASSERT_EQUAL_INT(LISTENING, sys.getState());
	TEST_ASSERT_EQUAL_STRING("INVALID_CMD", sender._lastMsg.c_str());
}

void test_getMessage()
{
	mockReceptor receptor;
	mockSender   sender;
	SendToPi_UC  sendUC(sender);
	System       sys;
	Commands     cmds = sys.getCommands();

	receptor._arrived = true;
	receptor._msg     = "PLATE_NEXT";
	CommandReceptorUC uc(receptor, cmds, sendUC);

	uc.checkForNewMessage(sys);
	TEST_ASSERT_EQUAL_STRING("PLATE_NEXT", uc.getMessage().c_str());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_checkForNewMessage_arrives);
	RUN_TEST(test_checkForNewMessage_no_message);
	RUN_TEST(test_checkForNewMessage_not_listening);
	RUN_TEST(test_interpreteMessage_valid);
	RUN_TEST(test_interpreteMessage_invalid);
	RUN_TEST(test_getMessage);
	return UNITY_END();
}
