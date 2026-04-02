#pragma once
#include "3_interface/IPico.hpp"
#include "3_interface/ICamera.hpp"
#include "3_interface/IDiskChecker.hpp"

class CaptureData_UC
{
	private:
		IPico  &_pico;
		ICamera      &_camera;
		IDiskChecker &_diskChecker;

	public:
		CaptureData_UC(IPico &pico, ICamera &camera, IDiskChecker &diskChecker);
		~CaptureData_UC() = default;

		bool execute();
};
