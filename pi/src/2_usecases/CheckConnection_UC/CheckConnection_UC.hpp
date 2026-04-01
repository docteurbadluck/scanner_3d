#pragma once
#include "3_interface/IPicoClient.hpp"
#include "3_interface/IHttpUploader.hpp"

class CheckConnection_UC
{
	private:
		IPicoClient   &_pico;
		IHttpUploader &_uploader;

	public:
		CheckConnection_UC(IPicoClient &pico, IHttpUploader &uploader);
		~CheckConnection_UC() = default;

		bool execute();
};
