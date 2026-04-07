#include "CaptureData_UC.hpp"

CaptureData_UC::CaptureData_UC(IPico &pico, ICamera &camera, IDiskChecker &diskChecker)
	: _pico(pico), _camera(camera), _diskChecker(diskChecker)
{
}
