#pragma once
#include "3_interface/IDiskChecker.hpp"

enum class CheckCaptureStorageResult
{
	OK,
	NO_SPACE
};

class CheckCaptureStorage_UC
{
	private:
		IDiskChecker &_diskChecker;

	public:
		CheckCaptureStorage_UC(IDiskChecker &diskChecker);
		~CheckCaptureStorage_UC() = default;

		CheckCaptureStorageResult execute();
};
