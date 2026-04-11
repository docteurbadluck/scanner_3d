#include "Camera_Driver.hpp"
#include <cstdlib>

bool Camera_Driver::capture()
{
	_hasPhoto = false;
	_data.clear();
	if (system(_buildCommand().c_str()) != 0)
		return false;
	if (!_readFile())
		return false;
	_hasPhoto = true;
	return true;
}
