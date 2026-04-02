#pragma once
#include <string>

class IComputer
{
	public:
		virtual ~IComputer() = default;
		virtual bool        isServerReachable() = 0;
		virtual bool        upload(const std::string &data) = 0;
		virtual std::string waitForOrder() = 0;
};
