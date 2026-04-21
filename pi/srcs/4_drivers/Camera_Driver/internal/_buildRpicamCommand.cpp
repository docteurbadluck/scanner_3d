#include "../Camera_Driver.hpp"

std::string Camera_Driver::_buildRpicamCommand() const
{
	return "rpicam-still --nopreview"
	       " -t "       + std::to_string(_cfg.timeout_ms) +
	       " --width "  + std::to_string(_cfg.width) +
	       " --height " + std::to_string(_cfg.height) +
	       " -o "       + _cfg.output_path;
}
