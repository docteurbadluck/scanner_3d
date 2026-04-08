#pragma once
#include "3_interface/ISender.hpp"
#include <cstdint>
#include <string>
#include <vector>

struct WsSender_DriverConfig
{
	std::string host    = "192.168.1.100";
	int         ws_port = 8082;
};

class WsSender_Driver : public ISender
{
public:
	explicit WsSender_Driver(const WsSender_DriverConfig &cfg);
	~WsSender_Driver() override;

	void send(const std::string &message) override;

private:
	WsSender_DriverConfig _cfg;
	int                   _fd;

	int  _tcpConnect();
	bool _connect();
	bool _sendWsHandshake();
	bool _readWsHandshakeOk();
	void _sendFrame(const std::string &payload);
	void _appendLength(std::vector<uint8_t> &frame, size_t len, const uint8_t mask[4]);
	void _writeFrame(const std::vector<uint8_t> &frame);
};
