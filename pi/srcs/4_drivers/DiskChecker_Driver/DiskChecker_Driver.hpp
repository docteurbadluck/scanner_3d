#pragma once
#include "3_interface/IDiskChecker.hpp"
#include <cstdint>
#include <string>

struct DiskChecker_DriverConfig
{
	std::string path      = "/";
	uint64_t    min_bytes = 10ULL * 1024 * 1024;  // 10 MB
};

class DiskChecker_Driver : public IDiskChecker
{
public:
	explicit DiskChecker_Driver(const DiskChecker_DriverConfig &cfg);
	~DiskChecker_Driver() override = default;

	bool hasEnoughSpace() override;

private:
	DiskChecker_DriverConfig _cfg;
};
