#include "Camera_Driver.hpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>

static void printResult(bool ok)
{
	std::time_t now = std::time(nullptr);
	std::tm    *t   = std::localtime(&now);
	std::printf("[%02d:%02d:%02d] result : %s\n",
	    t->tm_hour, t->tm_min, t->tm_sec, ok ? "OK" : "KO");
}

bool Camera_Driver::capture()
{
	_hasPhoto = false;
	_data.clear();
	const bool ok = system(_createTakePhotoCommand().c_str()) == 0 && _readFile();
	_hasPhoto = ok;
	printResult(ok);
	return ok;
}
