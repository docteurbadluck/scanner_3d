#include "SendData_UC.hpp"

SendData_UC::SendData_UC(ICamera &camera, IHttpUploader &uploader)
	: _camera(camera), _uploader(uploader)
{
}

bool SendData_UC::execute()
{
	if (!_camera.hasPhoto())
		return false;
	if (!_uploader.upload(_camera.getData()))
		return false;
	return true;
}
