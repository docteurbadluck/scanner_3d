#include "CheckCaptureStorage_UC.hpp"

CheckCaptureStorageResult CheckCaptureStorage_UC::execute()
{
	if (!_diskChecker.hasEnoughSpace())
		return CheckCaptureStorageResult::NO_SPACE;
	return CheckCaptureStorageResult::OK;
}
