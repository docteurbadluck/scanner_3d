#pragma once
#include "3_interface/IPicoClient.hpp"
#include "3_interface/ICamera.hpp"
#include "3_interface/IDiskChecker.hpp"

class CaptureData_UC
{
	private:
		IPicoClient  &_pico;
		ICamera      &_camera;
		IDiskChecker &_diskChecker;

	public:
		CaptureData_UC(IPicoClient &pico, ICamera &camera, IDiskChecker &diskChecker);
		~CaptureData_UC() = default;

		bool execute();
};
