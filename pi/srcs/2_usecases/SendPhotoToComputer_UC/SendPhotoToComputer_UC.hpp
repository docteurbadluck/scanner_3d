#pragma once
#include "3_interface/ICamera.hpp"
#include "3_interface/IComputer.hpp"

class SendPhotoToComputer_UC
{
	private:
		ICamera       &_camera;
		IComputer &_computer;

	public:
		SendPhotoToComputer_UC(ICamera &camera, IComputer &uploader);
		~SendPhotoToComputer_UC() = default;

		bool execute();
};
