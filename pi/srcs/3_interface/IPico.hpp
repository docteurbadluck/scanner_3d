#pragma once
#include <string>

class IPico
{
	public:
		virtual ~IPico() = default;
		virtual bool isReady() = 0;
		virtual bool sendCommand(const std::string &cmd) = 0;
		virtual bool setCameraPosition(const std::string &pose) = 0;
		virtual bool setArmPosition(const std::string &pose) = 0;
		virtual bool rotatePlateStep() = 0;
		virtual bool isStable() = 0;
		virtual std::string getPicoStatus() = 0;
	virtual std::string pingPico() = 0;
};
