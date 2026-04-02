#include "SendPhotoToComputer_UC.hpp"

SendPhotoToComputer_UC::SendPhotoToComputer_UC(ICamera &camera, IComputer &uploader)
	: _camera(camera), _uploader(uploader)
{
}

bool SendPhotoToComputer_UC::execute()
{
	if (!_camera.hasPhoto())
		return false;
	if (!_uploader.upload(_camera.getData()))
		return false;
	return true;
}
