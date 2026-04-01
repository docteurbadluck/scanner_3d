#pragma once
#include "3_interface/ICamera.hpp"
#include "3_interface/IHttpUploader.hpp"

class SendData_UC
{
	private:
		ICamera       &_camera;
		IHttpUploader &_uploader;

	public:
		SendData_UC(ICamera &camera, IHttpUploader &uploader);
		~SendData_UC() = default;

		bool execute();
};
