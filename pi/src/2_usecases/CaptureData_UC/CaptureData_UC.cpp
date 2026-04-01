#include "CaptureData_UC.hpp"

CaptureData_UC::CaptureData_UC(IPicoClient &pico, ICamera &camera, IDiskChecker &diskChecker)
	: _pico(pico), _camera(camera), _diskChecker(diskChecker)
{
}

bool CaptureData_UC::execute()
{
	if (!_diskChecker.hasEnoughSpace())
		return false;
	if (!_pico.sendCommand("CAPTURE"))
		return false;
	if (!_camera.capture())
		return false;
	return true;
}
