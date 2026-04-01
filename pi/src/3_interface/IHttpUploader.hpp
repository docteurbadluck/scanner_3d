#pragma once
#include <string>

class IHttpUploader
{
	public:
		virtual ~IHttpUploader() = default;
		virtual bool        isServerReachable() = 0;
		virtual bool        upload(const std::string &data) = 0;
		virtual std::string waitForOrder() = 0;
};
