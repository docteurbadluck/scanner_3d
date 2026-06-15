#pragma once
#include "3_interface/ICamera.hpp"
#include "3_interface/IDiskChecker.hpp"
#include "2_usecases/VibrationMonitor_UC/VibrationMonitor_UC.hpp"

class CaptureData_UC
{
	private:
		ICamera            &_camera;
		IDiskChecker       &_diskChecker;
		VibrationMonitor_UC &_vibration;

	public:
		CaptureData_UC(ICamera &camera, IDiskChecker &diskChecker, VibrationMonitor_UC &vibration);
		~CaptureData_UC() = default;

		bool execute();
};
