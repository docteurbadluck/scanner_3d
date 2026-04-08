#include "../Pico_Driver.hpp"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

void Pico_Driver::_openPort()
{
	_fd = open(_cfg.port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	if (_fd < 0)
		return;
	struct termios tty{};
	tcgetattr(_fd, &tty);
	cfsetspeed(&tty, B115200);
	tty.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CRTSCTS);
	tty.c_cflag |= (CLOCAL | CREAD | CS8);
	tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_oflag &= ~OPOST;
	tcsetattr(_fd, TCSANOW, &tty);
}
