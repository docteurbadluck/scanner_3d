#pragma once
#include "3_interface/IPico.hpp"
#include <string>

struct Pico_DriverConfig
{
	std::string port            = "/dev/ttyACM0";
	int         read_timeout_ms = 2000;
};

class Pico_Driver : public IPico
{
public:
	explicit Pico_Driver(const Pico_DriverConfig &cfg);
	~Pico_Driver() override;

	bool        isReady()                                   override;
	bool        sendCommand(const std::string &cmd)         override;
	bool        setCameraPosition(const std::string &pose)  override;
	bool        setArmPosition(const std::string &pose)     override;
	bool        rotatePlateStep()                           override;
	bool        isStable()                                  override;
	std::string getPicoStatus()                             override;

private:
	Pico_DriverConfig _cfg;
	int               _fd;

	void        _openPort();
	bool        _writeLine(const std::string &line);
	bool        _readChar(char &ch);
	std::string _readResponse();
	bool        _isSuccess(const std::string &response);
};
