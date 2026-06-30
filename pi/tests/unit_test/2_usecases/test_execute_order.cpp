#include <cstring>
#include "3_interface/IPico.hpp"
#include "3_interface/ICamera.hpp"
#include "3_interface/IDiskChecker.hpp"
#include "3_interface/IComputer.hpp"
#include "3_interface/ISender.hpp"
#include "3_interface/IAccelerometer.hpp"
#include "2_usecases/VibrationMonitor_UC/VibrationMonitor_UC.hpp"
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
    bool        _armRes = true;
    bool        _camRes = true;
    std::string _status = "{\"type\":\"state\",\"state\":\"READY\"}";
    std::string _lastCommand;
    std::string _lastArmPose;
    std::string _lastCamPose;
    int         _rotateCalls = 0;
    bool        _initialPosRes = true;
    int         _initialPosCalls = 0;
    bool        isReady()                               override { return _res; }
    bool        sendCommand(const std::string &cmd)     override { _lastCommand = cmd; return _res; }
    bool        setCameraPosition(const std::string &p) override { _lastCamPose = p; return _camRes; }
    bool        setArmPosition(const std::string &p)    override { _lastArmPose = p; return _armRes; }
    bool        rotatePlateStep()                       override { _rotateCalls++; return _res; }
    bool        goInitialPos()                          override { _initialPosCalls++; return _initialPosRes; }
    std::string getPicoStatus()                         override { return _status; }
    std::string pingPico()                              override { return JsonMessage::makePingPicoResponse(0); }
    std::string testHardware()                          override { return "{}"; }
};

class mockAccelerometer : public IAccelerometer
{
public:
    float readMagnitude() override { return 0.0f; }
};

class mockCamera : public ICamera
{
public:
    bool        _captureRes = true;
    bool        _hasPhoto   = true;
    std::string _data       = "photo";
    bool        capture()        override { return _captureRes; }
    bool        hasPhoto() const override { return _hasPhoto; }
    std::string getData()  const override { return _data; }
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
    bool        isServerReachable()             override { return _res; }
    bool        upload(const std::string &)     override { return _res; }
    std::string waitForOrder()                  override { return _order; }
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
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    run_cmd(exec, sys, "PING");
    TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("PONG", "PING").c_str(), sender._lastMsg.c_str());
}

void test_execute_get_status()
{
    mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
    mockHttpUploader uploader; mockSender sender;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    sys.ready();
    run_cmd(exec, sys, "GET_STATUS");
    TEST_ASSERT_EQUAL_STRING(JsonMessage::makeState("READY").c_str(), sender._lastMsg.c_str());
}

void test_execute_get_pico_status()
{
    mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
    mockHttpUploader uploader; mockSender sender;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    pico._status = "{\"type\":\"state\",\"state\":\"READY\"}";
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    run_cmd(exec, sys, "GET_PICO_STATUS");
    TEST_ASSERT_EQUAL_STRING("{\"type\":\"pico_status\",\"state\":\"READY\"}", sender._lastMsg.c_str());
}

void test_execute_start_capture_success()
{
    mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
    mockHttpUploader uploader; mockSender sender;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    run_cmd(exec, sys, "TAKE_PHOTO");
    TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("DONE", "TAKE_PHOTO").c_str(), sender._lastMsg.c_str());
}

void test_execute_start_capture_fail()
{
    mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
    mockHttpUploader uploader; mockSender sender;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    cam._captureRes = false;
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    run_cmd(exec, sys, "TAKE_PHOTO");
    TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("FAIL", "TAKE_PHOTO").c_str(), sender._lastMsg.c_str());
}

void test_execute_pong_noop()
{
    mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
    mockHttpUploader uploader; mockSender sender;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    run_cmd(exec, sys, "PONG");
    TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("DONE", "PONG").c_str(), sender._lastMsg.c_str());
}

void test_execute_ping_pico()
{
    mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
    mockHttpUploader uploader; mockSender sender;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    run_cmd(exec, sys, "PING_PICO");
    TEST_ASSERT_NOT_NULL(strstr(sender._lastMsg.c_str(), "PING_PICO"));
    TEST_ASSERT_NOT_NULL(strstr(sender._lastMsg.c_str(), "ms"));
}

void test_execute_unknown_command()
{
    mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
    mockHttpUploader uploader; mockSender sender;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    run_cmd(exec, sys, "UNKNOWN_CMD");
    TEST_ASSERT_EQUAL_STRING(JsonMessage::makeError("INVALID_CMD").c_str(), sender._lastMsg.c_str());
}

static void run_position_test(const std::string &cmd, const char *armPose, const char *camPose)
{
    mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
    mockHttpUploader uploader; mockSender sender;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    run_cmd(exec, sys, cmd);
    TEST_ASSERT_EQUAL_STRING(armPose, pico._lastArmPose.c_str());
    TEST_ASSERT_EQUAL_STRING(camPose, pico._lastCamPose.c_str());
    TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("DONE", cmd).c_str(), sender._lastMsg.c_str());
}

void test_execute_position_a() { run_position_test("POSITION_A", "ARM_UP", "CAM_A"); }
void test_execute_position_b() { run_position_test("POSITION_B", "ARM_UP", "CAM_B"); }
void test_execute_position_c() { run_position_test("POSITION_C", "ARM_DOWN", "CAM_C"); }
void test_execute_position_d() { run_position_test("POSITION_D", "ARM_DOWN", "CAM_D"); }

void test_execute_position_fail_when_arm_fails()
{
    mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
    mockHttpUploader uploader; mockSender sender;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    pico._armRes = false;
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    run_cmd(exec, sys, "POSITION_A");
    TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("FAIL", "POSITION_A").c_str(), sender._lastMsg.c_str());
}

void test_execute_plate_next()
{
    mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
    mockHttpUploader uploader; mockSender sender;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    run_cmd(exec, sys, "PLATE_NEXT");
    TEST_ASSERT_EQUAL(1, pico._rotateCalls);
    TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("DONE", "PLATE_NEXT").c_str(), sender._lastMsg.c_str());
}

void test_execute_plate_next_fail()
{
    mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
    mockHttpUploader uploader; mockSender sender;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    pico._res = false;
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    run_cmd(exec, sys, "PLATE_NEXT");
    TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("FAIL", "PLATE_NEXT").c_str(), sender._lastMsg.c_str());
}

void test_execute_initial_pos()
{
    mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
    mockHttpUploader uploader; mockSender sender;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    run_cmd(exec, sys, "INITIAL_POS");
    TEST_ASSERT_EQUAL(1, pico._initialPosCalls);
    TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("DONE", "INITIAL_POS").c_str(), sender._lastMsg.c_str());
}

void test_execute_initial_pos_fail()
{
    mockPicoClient pico; mockCamera cam; mockDiskChecker disk;
    mockHttpUploader uploader; mockSender sender;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    pico._initialPosRes = false;
    CaptureData_UC    capture(cam, disk, vib, [](uint32_t) {});
    SendPhotoToComputer_UC sendData(cam, uploader);
    SendToComputer_UC sendUC(sender);
    ExecuteOrder_UC exec(capture, sendData, sendUC, pico);
    System sys;
    run_cmd(exec, sys, "INITIAL_POS");
    TEST_ASSERT_EQUAL_STRING(JsonMessage::makeResponse("FAIL", "INITIAL_POS").c_str(), sender._lastMsg.c_str());
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
    RUN_TEST(test_execute_position_a);
    RUN_TEST(test_execute_position_b);
    RUN_TEST(test_execute_position_c);
    RUN_TEST(test_execute_position_d);
    RUN_TEST(test_execute_position_fail_when_arm_fails);
    RUN_TEST(test_execute_plate_next);
    RUN_TEST(test_execute_plate_next_fail);
    RUN_TEST(test_execute_initial_pos);
    RUN_TEST(test_execute_initial_pos_fail);
    return UNITY_END();
}
