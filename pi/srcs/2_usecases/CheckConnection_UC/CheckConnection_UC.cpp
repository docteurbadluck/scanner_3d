#include "CheckConnection_UC.hpp"

CheckConnection_UC::CheckConnection_UC(IPico &pico, IComputer &uploader)
	: _pico(pico), _uploader(uploader)
{
}

ConnectionStatus CheckConnection_UC::execute()
{
	if (!_pico.isReady())
		return ConnectionStatus::PICO_UNREACHABLE;
	if (!_uploader.isServerReachable())
		return ConnectionStatus::SERVER_UNREACHABLE;
	return ConnectionStatus::OK;
}
