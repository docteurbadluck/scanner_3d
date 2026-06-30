#include "3_interface/IAccelerometer.hpp"
#include "3_interface/ICamera.hpp"
#include "3_interface/IDiskChecker.hpp"
#include "2_usecases/VibrationMonitor_UC/VibrationMonitor_UC.hpp"
#include "2_usecases/CaptureData_UC/CaptureData_UC.hpp"
#include <string>

class mockAccelerometer : public IAccelerometer
{
public:
    float _magnitude = 0.0f;
    float readMagnitude() override { return _magnitude; }
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
    mockAccelerometer acc1, acc2;
    VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    CaptureData_UC uc(cam, disk, vib, [](uint32_t) {});
    TEST_ASSERT_TRUE(uc.execute());
}

void test_CaptureData_no_disk_space()
{
    mockCamera cam; mockDiskChecker disk;
    mockAccelerometer acc1, acc2;
    VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    disk._res = false;
    CaptureData_UC uc(cam, disk, vib, [](uint32_t) {});
    TEST_ASSERT_FALSE(uc.execute());
}

void test_CaptureData_camera_fails()
{
    mockCamera cam; mockDiskChecker disk;
    mockAccelerometer acc1, acc2;
    VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    cam._res = false;
    CaptureData_UC uc(cam, disk, vib, [](uint32_t) {});
    TEST_ASSERT_FALSE(uc.execute());
}

void test_CaptureData_unstable_never_blocks_forever()
{
    mockCamera cam; mockDiskChecker disk;
    mockAccelerometer acc1, acc2;
    acc1._magnitude = 1.0f;
    int sleepCalls = 0;
    VibrationMonitor_UC vib(acc1, acc2, [](uint32_t) {});
    CaptureData_UC uc(cam, disk, vib, [&sleepCalls](uint32_t) { ++sleepCalls; });
    TEST_ASSERT_FALSE(uc.execute());
    TEST_ASSERT_GREATER_THAN(0, sleepCalls);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_CaptureData_success);
    RUN_TEST(test_CaptureData_no_disk_space);
    RUN_TEST(test_CaptureData_camera_fails);
    RUN_TEST(test_CaptureData_unstable_never_blocks_forever);
    return UNITY_END();
}
