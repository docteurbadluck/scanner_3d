#pragma once
#include <string>

class ICamera
{
	public:
		virtual ~ICamera() = default;
		virtual bool        capture() = 0;
		virtual bool        hasPhoto() const = 0;
		virtual std::string getData() const = 0;
};
