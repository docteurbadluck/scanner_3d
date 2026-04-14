#include "3_interface/IPico.hpp"
#include "3_interface/ICamera.hpp"
#include "3_interface/IDiskChecker.hpp"
#include "3_interface/IComputer.hpp"
#include "3_interface/ISender.hpp"
#include "2_usecases/CaptureData_UC/CaptureData_UC.hpp"
#include "2_usecases/SendPhotoToComputer_UC/SendPhotoToComputer_UC.hpp"
#include "2_usecases/SendToComputer_UC/SendToComputer_UC.hpp"
#include "2_usecases/ExecuteOrder_UC/ExecuteOrder_UC.hpp"
#include "1_domain/System/System.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"
#include <string>

class mockPicoClient : public IPico
{
public:
	bool        _res = true;
	std::string _status = "{\"type\":\"state\",\"state\":\"READY\"}";
	std::string _lastCommand;
	bool isReady() override { return _res; }
	bool sendCommand(const std::string &command) override { _lastCommand = command; return _res; }
	bool setCameraPosition(const std::string &) override { return _res; }
	bool setArmPosition(const std::string &) override { return _res; }
	bool rotatePlateStep() override { return _res; }
	bool isStable() override { return _res; }
	std::string getPicoStatus() override { return _status; }
	std::string pingPico() override { return JsonMessage::makePingPicoResponse(0); }
};

class mockCamera : public ICamera
{
public:
	bool        _captureRes = true;
	bool        _hasPhoto   = true;
	std::string _data       = "photo";
	bool        capture() override { return _captureRes; }
	bool        hasPhoto() const override { return _hasPhoto; }
	std::string getData() const override { return _data; }
};

class mockDiskChecker : public IDiskChecker
{
public:
	bool _res = true;
	bool hasEnoughSpace() override { return _res; }
};

class mockHttpUploader : public IComputer
{
public:
	bool        _res = true;
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

static void run_cmd(ExecuteOrder_UC &exec, System &sys, const std::string &cmd)
{
	sys.setCommandToExecute(cmd);
	exec.execute(sys);
}

void test_execute_ping()
{
	mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
	mockHttpUploader uploader; mockSender sender;
	CaptureData_UC    capture(pico, cam, disk);
	SendPhotoToComputer_UC       sendData(cam, uploader);
	SendToComputer_UC sendUC(sender);
	ExecuteOrder_UC   exec(capture, sendData, sendUC, pico);
	System sys;

	run_cmd(exec, sys, "PING");
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("PONG", "PING").c_str(), sender._lastMsg.c_str());
}

void test_execute_get_status()
{
	mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
	mockHttpUploader uploader; mockSender sender;
	CaptureData_UC    capture(pico, cam, disk);
	SendPhotoToComputer_UC       sendData(cam, uploader);
	SendToComputer_UC sendUC(sender);
	ExecuteOrder_UC   exec(capture, sendData, sendUC, pico);
	System sys;
	sys.ready();

	run_cmd(exec, sys, "GET_STATUS");
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeState("READY").c_str(), sender._lastMsg.c_str());
}

void test_execute_get_pico_status()
{
	mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
	mockHttpUploader uploader; mockSender sender;
	pico._status = "{\"type\":\"state\",\"state\":\"READY\"}";
	CaptureData_UC    capture(pico, cam, disk);
	SendPhotoToComputer_UC       sendData(cam, uploader);
	SendToComputer_UC sendUC(sender);
	ExecuteOrder_UC   exec(capture, sendData, sendUC, pico);
	System sys;

	run_cmd(exec, sys, "GET_PICO_STATUS");
	TEST_ASSERT_EQUAL_STRING("{\"type\":\"state\",\"state\":\"READY\"}", sender._lastMsg.c_str());
}

void test_execute_start_capture_success()
{
	mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
	mockHttpUploader uploader; mockSender sender;
	CaptureData_UC    capture(pico, cam, disk);
	SendPhotoToComputer_UC       sendData(cam, uploader);
	SendToComputer_UC sendUC(sender);
	ExecuteOrder_UC   exec(capture, sendData, sendUC, pico);
	System sys;

	run_cmd(exec, sys, "START_CAPTURE");
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("DONE", "START_CAPTURE").c_str(), sender._lastMsg.c_str());
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeCommand("CAPTURE").c_str(), pico._lastCommand.c_str());
}

void test_execute_start_capture_fail()
{
	mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
	mockHttpUploader uploader; mockSender sender;
	pico._res = false;
	CaptureData_UC    capture(pico, cam, disk);
	SendPhotoToComputer_UC       sendData(cam, uploader);
	SendToComputer_UC sendUC(sender);
	ExecuteOrder_UC   exec(capture, sendData, sendUC, pico);
	System sys;

	run_cmd(exec, sys, "START_CAPTURE");
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("FAIL", "START_CAPTURE").c_str(), sender._lastMsg.c_str());
}

void test_execute_pong_noop()
{
	mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
	mockHttpUploader uploader; mockSender sender;
	CaptureData_UC    capture(pico, cam, disk);
	SendPhotoToComputer_UC       sendData(cam, uploader);
	SendToComputer_UC sendUC(sender);
	ExecuteOrder_UC   exec(capture, sendData, sendUC, pico);
	System sys;

	run_cmd(exec, sys, "PONG");
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("DONE", "PONG").c_str(), sender._lastMsg.c_str());
}

void test_execute_ping_pico()
{
	mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
	mockHttpUploader uploader; mockSender sender;
	CaptureData_UC    capture(pico, cam, disk);
	SendPhotoToComputer_UC       sendData(cam, uploader);
	SendToComputer_UC sendUC(sender);
	ExecuteOrder_UC   exec(capture, sendData, sendUC, pico);
	System sys;

	run_cmd(exec, sys, "PING_PICO");
	TEST_ASSERT_NOT_NULL(strstr(sender._lastMsg.c_str(), "PING_PICO"));
	TEST_ASSERT_NOT_NULL(strstr(sender._lastMsg.c_str(), "ms"));
}

void test_execute_unknown_command()
{
	mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
	mockHttpUploader uploader; mockSender sender;
	CaptureData_UC    capture(pico, cam, disk);
	SendPhotoToComputer_UC       sendData(cam, uploader);
	SendToComputer_UC sendUC(sender);
	ExecuteOrder_UC   exec(capture, sendData, sendUC, pico);
	System sys;

	run_cmd(exec, sys, "UNKNOWN_CMD");
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeError("INVALID_CMD").c_str(), sender._lastMsg.c_str());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_execute_ping);
	RUN_TEST(test_execute_get_status);
	RUN_TEST(test_execute_get_pico_status);
	RUN_TEST(test_execute_start_capture_success);
	RUN_TEST(test_execute_start_capture_fail);
	RUN_TEST(test_execute_pong_noop);
	RUN_TEST(test_execute_ping_pico);
	RUN_TEST(test_execute_unknown_command);
	return UNITY_END();
}
