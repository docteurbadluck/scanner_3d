#include "SendPhotoToComputer_UC.hpp"

SendPhotoToComputer_UC::SendPhotoToComputer_UC(ICamera &camera, IComputer &uploader)
	: _camera(camera), _computer(uploader)
{
}
