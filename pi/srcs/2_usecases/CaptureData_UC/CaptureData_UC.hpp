#pragma once
#include "3_interface/ICamera.hpp"
#include "3_interface/IDiskChecker.hpp"
#include "2_usecases/VibrationMonitor_UC/VibrationMonitor_UC.hpp"
#include <cstdint>
#include <functional>

class CaptureData_UC
{
	private:
		ICamera            &_camera;
		IDiskChecker       &_diskChecker;
		VibrationMonitor_UC &_vibration;
		std::function<void(uint32_t)> _sleep_ms;

		static constexpr int      MAX_STABILITY_ATTEMPTS  = 50;
		static constexpr uint32_t STABILITY_RETRY_DELAY_MS = 100;

		bool _waitForStability();

	public:
		CaptureData_UC(ICamera &camera, IDiskChecker &diskChecker,
			VibrationMonitor_UC &vibration, std::function<void(uint32_t)> sleep_ms);
		~CaptureData_UC() = default;

		bool execute();
};
