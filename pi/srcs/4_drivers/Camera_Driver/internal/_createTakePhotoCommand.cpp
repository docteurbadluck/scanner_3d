#include "../Camera_Driver.hpp"

std::string Camera_Driver::_createTakePhotoCommand() const
{
	const std::string cmd = _buildRpicamCommand();
	return "tmpf=$(mktemp); " + cmd + " 2>\"$tmpf\"; "
	       "r=$?; [ $r -ne 0 ] && cat \"$tmpf\" >&2; rm -f \"$tmpf\"; exit $r";
}
