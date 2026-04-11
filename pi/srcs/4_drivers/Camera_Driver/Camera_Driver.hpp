#pragma once
#include "3_interface/ICamera.hpp"
#include <string>

struct Camera_DriverConfig
{
	std::string output_path = "/tmp/scan_photo.jpg";
	int         width       = 2592;
	int         height      = 1944;
	int         timeout_ms  = 1000;
};

class Camera_Driver : public ICamera
{
public:
	explicit Camera_Driver(const Camera_DriverConfig &cfg);
	~Camera_Driver() override = default;

	bool        capture()        override;
	bool        hasPhoto() const override;
	std::string getData()  const override;

private:
	Camera_DriverConfig _cfg;
	bool                _hasPhoto;
	std::string         _data;

	std::string _buildCommand() const;
	bool        _readFile();
};
