#include "CaptureData_UC.hpp"
#include "1_domain/PicoJson.hpp"

CaptureData_UC::CaptureData_UC(IPico &pico, ICamera &camera, IDiskChecker &diskChecker)
	: _pico(pico), _camera(camera), _diskChecker(diskChecker)
{
}

bool CaptureData_UC::execute()
{
	if (!_diskChecker.hasEnoughSpace())
		return false;
	if (!_pico.sendCommand(PicoJson::makeCommand("CAPTURE")))
		return false;
	if (!_camera.capture())
		return false;
	return true;
}
