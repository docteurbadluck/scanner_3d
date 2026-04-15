#include "2_usecases/CaptureData_UC/CaptureData_UC.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"
#include <string>

class mockPicoClient : public IPico
{
	public:
		mockPicoClient() {}
		~mockPicoClient() = default;
		bool _res;
		std::string _lastCommand;
		bool isReady() override { return _res; }
		bool sendCommand(const std::string &command) override { _lastCommand = command; return _res; }
		bool setCameraPosition(const std::string &) override { return _res; }
		bool setArmPosition(const std::string &) override { return _res; }
		bool rotatePlateStep() override { return _res; }
		bool isStable() override { return _res; }
		std::string getPicoStatus() override { return "{\"type\":\"state\",\"state\":\"READY\"}"; }
		std::string pingPico() override { return "{}"; }
};

class mockCamera : public ICamera
{
	public:
		mockCamera() {}
		~mockCamera() = default;
		bool _res;
		bool        capture() override { return _res; }
		bool        hasPhoto() const override { return _res; }
		std::string getData() const override { return "photo_data"; }
};

class mockDiskChecker : public IDiskChecker
{
	public:
		mockDiskChecker() {}
		~mockDiskChecker() = default;
		bool _res;
		bool hasEnoughSpace() override { return _res; }
};

extern "C"
{
	#include "unity.h"

	void setUp(void)    {}
	void tearDown(void) {}
}

void test_CaptureData_success()
{
	mockPicoClient  pico;
	mockCamera      camera;
	mockDiskChecker disk;
	pico._res   = true;
	camera._res = true;
	disk._res   = true;
	CaptureData_UC uc(pico, camera, disk);
	TEST_ASSERT_TRUE(uc.execute());
	TEST_ASSERT_EQUAL_STRING(JsonMessage::makeCommand("CAPTURE").c_str(), pico._lastCommand.c_str());
}

void test_CaptureData_no_disk_space()
{
	mockPicoClient  pico;
	mockCamera      camera;
	mockDiskChecker disk;
	pico._res   = true;
	camera._res = true;
	disk._res   = false;
	CaptureData_UC uc(pico, camera, disk);
	TEST_ASSERT_FALSE(uc.execute());
}

void test_CaptureData_pico_fails()
{
	mockPicoClient  pico;
	mockCamera      camera;
	mockDiskChecker disk;
	pico._res   = false;
	camera._res = true;
	disk._res   = true;
	CaptureData_UC uc(pico, camera, disk);
	TEST_ASSERT_FALSE(uc.execute());
}

void test_CaptureData_camera_fails()
{
	mockPicoClient  pico;
	mockCamera      camera;
	mockDiskChecker disk;
	pico._res   = true;
	camera._res = false;
	disk._res   = true;
	CaptureData_UC uc(pico, camera, disk);
	TEST_ASSERT_FALSE(uc.execute());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_CaptureData_success);
	RUN_TEST(test_CaptureData_no_disk_space);
	RUN_TEST(test_CaptureData_pico_fails);
	RUN_TEST(test_CaptureData_camera_fails);
	return UNITY_END();
}
