#pragma once
#include "3_interface/IPico.hpp"
#include "3_interface/IComputer.hpp"

enum class ConnectionStatus { OK, PICO_UNREACHABLE, SERVER_UNREACHABLE };

class CheckConnection_UC
{
	private:
		IPico   &_pico;
		IComputer &_uploader;

	public:
		CheckConnection_UC(IPico &pico, IComputer &uploader);
		~CheckConnection_UC() = default;

		[[nodiscard]] ConnectionStatus execute();
};
