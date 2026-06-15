#include "3_interface/IAccelerometer.hpp"
#include "3_interface/ICamera.hpp"
#include "3_interface/IDiskChecker.hpp"
#include "2_usecases/VibrationMonitor_UC/VibrationMonitor_UC.hpp"
#include "2_usecases/CaptureData_UC/CaptureData_UC.hpp"
#include <string>

class mockAccelerometer : public IAccelerometer
{
public:
    float readMagnitude() override { return 0.0f; }
};

class mockCamera : public ICamera
{
public:
    bool _res = true;
    bool        capture()        override { return _res; }
    bool        hasPhoto() const override { return _res; }
    std::string getData()  const override { return "photo_data"; }
};

class mockDiskChecker : public IDiskChecker
{
public:
    bool _res = true;
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
    mockCamera cam; mockDiskChecker disk;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2);
    CaptureData_UC uc(cam, disk, vib);
    TEST_ASSERT_TRUE(uc.execute());
}

void test_CaptureData_no_disk_space()
{
    mockCamera cam; mockDiskChecker disk;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2);
    disk._res = false;
    CaptureData_UC uc(cam, disk, vib);
    TEST_ASSERT_FALSE(uc.execute());
}

void test_CaptureData_camera_fails()
{
    mockCamera cam; mockDiskChecker disk;
    mockAccelerometer acc1, acc2; VibrationMonitor_UC vib(acc1, acc2);
    cam._res = false;
    CaptureData_UC uc(cam, disk, vib);
    TEST_ASSERT_FALSE(uc.execute());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_CaptureData_success);
    RUN_TEST(test_CaptureData_no_disk_space);
    RUN_TEST(test_CaptureData_camera_fails);
    return UNITY_END();
}
