#include "../Pico_Driver.hpp"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

static int openFirstAvailable(const std::string &base)
{
	for (int i = 0; i < 4; ++i)
	{
		const std::string path = base.substr(0, base.size() - 1) + std::to_string(i);
		const int fd = open(path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
		if (fd >= 0)
			return fd;
	}
	return -1;
}

static void configurePort(int fd)
{
	struct termios tty{};
	tcgetattr(fd, &tty);
	cfsetspeed(&tty, B115200);
	tty.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CRTSCTS | HUPCL);
	tty.c_cflag |= (CLOCAL | CREAD | CS8);
	tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_oflag &= ~OPOST;
	tcsetattr(fd, TCSANOW, &tty);
	tcflush(fd, TCIOFLUSH);
}

void Pico_Driver::_openPort()
{
	_fd = open(_cfg.port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	if (_fd < 0)
		_fd = openFirstAvailable(_cfg.port);
	if (_fd < 0)
		return;
	configurePort(_fd);
	usleep(200000);
}
