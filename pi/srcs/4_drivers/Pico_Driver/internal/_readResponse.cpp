#include "../Pico_Driver.hpp"

std::string Pico_Driver::_readResponse()
{
	if (_fd < 0)
		return "";
	std::string result;
	char        ch;
	while (_readChar(ch))
	{
		if (ch == '\n')
			break;
		result += ch;
	}
	return result;
}
