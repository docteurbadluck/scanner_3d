#pragma once
#include "3_interface/IComputer.hpp"
#include "3_interface/ISender.hpp"
#include <cstdint>
#include <string>
#include <vector>

struct Computer_DriverConfig
{
	std::string host      = "192.168.1.100";
	int         ws_port   = 8082;
	int         http_port = 8080;
};

class Computer_Driver : public IComputer, public ISender
{
public:
	explicit Computer_Driver(const Computer_DriverConfig &cfg);
	~Computer_Driver() override;

	bool        isServerReachable() override;
	bool        upload(const std::string &data) override;
	std::string waitForOrder()      override;
	void        send(const std::string &message) override;

private:
	Computer_DriverConfig _cfg;
	int                   _fd;

	int         _tcpConnect(int port);
	bool        _connect();
	bool        _sendWsHandshake();
	bool        _readWsHandshakeOk();
	std::string _readFrame();
	std::string _readFramePayload(size_t len);
	bool        _sendHttpPost(int fd, const std::string &data);
	std::string _extractCommand(const std::string &json);
	void        _appendLen(std::vector<uint8_t> &frame, size_t len, const uint8_t mask[4]);
	void        _sendFrame(const std::string &payload);
	void        _writeFrameBytes(const std::vector<uint8_t> &frame);
};
