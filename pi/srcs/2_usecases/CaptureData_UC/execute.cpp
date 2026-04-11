#include "CaptureData_UC.hpp"
#include "3_interface/JsonMessage/JsonMessage.hpp"

bool CaptureData_UC::execute()
{
	if (!_diskChecker.hasEnoughSpace())
		return false;
	if (!_pico.sendCommand(JsonMessage::makeCommand("CAPTURE")))
		return false;
	if (!_camera.capture())
		return false;
	return true;
}
