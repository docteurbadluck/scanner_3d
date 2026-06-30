#include "CaptureData_UC.hpp"

CaptureData_UC::CaptureData_UC(ICamera &camera, IDiskChecker &diskChecker,
    VibrationMonitor_UC &vibration, std::function<void(uint32_t)> sleep_ms)
    : _camera(camera), _diskChecker(diskChecker), _vibration(vibration), _sleep_ms(sleep_ms)
{
}
