#pragma once
#include <string>
#include "3_interface/IHttpUploader.hpp"

class ListenForOrder_UC
{
	private:
		IHttpUploader &_uploader;

	public:
		ListenForOrder_UC(IHttpUploader &uploader);
		~ListenForOrder_UC() = default;

		std::string waitForOrder();
};
