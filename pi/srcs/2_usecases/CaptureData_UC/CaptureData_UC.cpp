#include "CaptureData_UC.hpp"

CaptureData_UC::CaptureData_UC(ICamera &camera, IDiskChecker &diskChecker, VibrationMonitor_UC &vibration)
    : _camera(camera), _diskChecker(diskChecker), _vibration(vibration)
{
}
