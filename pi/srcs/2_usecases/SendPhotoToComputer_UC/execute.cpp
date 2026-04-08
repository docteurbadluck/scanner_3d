#include "SendPhotoToComputer_UC.hpp"

bool SendPhotoToComputer_UC::execute()
{
	if (!_camera.hasPhoto())
		return false;
	if (!_uploader.upload(_camera.getData()))
		return false;
	return true;
}
