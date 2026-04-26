#include "CheckConnection_UC.hpp"

CheckConnection_UC::CheckConnection_UC(IPico &pico, IComputer &uploader)
	: _pico(pico), _computer(uploader)
{
}
