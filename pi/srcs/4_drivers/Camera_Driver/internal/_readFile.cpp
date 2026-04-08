#include "../Camera_Driver.hpp"
#include <fstream>

bool Camera_Driver::_readFile()
{
	std::ifstream file(_cfg.output_path, std::ios::binary);
	if (!file.is_open())
		return false;
	_data.assign(std::istreambuf_iterator<char>(file),
	             std::istreambuf_iterator<char>());
	return !_data.empty();
}
