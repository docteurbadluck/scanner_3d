#include "DiskChecker_Driver.hpp"
#include <sys/statvfs.h>

bool DiskChecker_Driver::hasEnoughSpace()
{
	struct statvfs stat;
	if (statvfs(_cfg.path.c_str(), &stat) != 0)
		return false;
	uint64_t available = static_cast<uint64_t>(stat.f_bavail) * stat.f_frsize;
	return available >= _cfg.min_bytes;
}
