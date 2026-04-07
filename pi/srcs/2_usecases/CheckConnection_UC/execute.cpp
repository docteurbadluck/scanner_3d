#include "CheckConnection_UC.hpp"

ConnectionStatus CheckConnection_UC::execute()
{
	if (!_pico.isReady())
		return ConnectionStatus::PICO_UNREACHABLE;
	if (!_uploader.isServerReachable())
		return ConnectionStatus::SERVER_UNREACHABLE;
	return ConnectionStatus::OK;
}
