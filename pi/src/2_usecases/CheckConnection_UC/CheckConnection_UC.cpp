#include "CheckConnection_UC.hpp"

CheckConnection_UC::CheckConnection_UC(IPicoClient &pico, IHttpUploader &uploader)
	: _pico(pico), _uploader(uploader)
{
}

bool CheckConnection_UC::execute()
{
	if (!_pico.isReady())
		return false;
	if (!_uploader.isServerReachable())
		return false;
	return true;
}
