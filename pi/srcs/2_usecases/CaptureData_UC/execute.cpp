#include "CaptureData_UC.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

bool CaptureData_UC::execute()
{
    if (!_diskChecker.hasEnoughSpace())
        return false;
    while (!_vibration.isStable()) {}
    if (!_camera.capture())
        return false;
    return true;
}
