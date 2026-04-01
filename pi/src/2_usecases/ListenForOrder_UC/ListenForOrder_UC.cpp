#include "ListenForOrder_UC.hpp"

ListenForOrder_UC::ListenForOrder_UC(IHttpUploader &uploader)
	: _uploader(uploader)
{
}

std::string ListenForOrder_UC::waitForOrder()
{
	return _uploader.waitForOrder();
}
