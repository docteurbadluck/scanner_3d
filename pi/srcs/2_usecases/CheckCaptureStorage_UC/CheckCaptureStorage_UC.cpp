#include "CheckCaptureStorage_UC.hpp"

CheckCaptureStorage_UC::CheckCaptureStorage_UC(IDiskChecker &diskChecker)
	: _diskChecker(diskChecker)
{
}

CheckCaptureStorageResult CheckCaptureStorage_UC::execute()
{
	if (!_diskChecker.hasEnoughSpace())
		return CheckCaptureStorageResult::NO_SPACE;
	return CheckCaptureStorageResult::OK;
}
