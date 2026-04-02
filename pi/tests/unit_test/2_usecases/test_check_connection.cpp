#include "2_usecases/CheckConnection_UC/CheckConnection_UC.hpp"
#include <string>

class mockPicoClient : public IPico
{
	public:
		mockPicoClient() {}
		~mockPicoClient() = default;
		bool _res;
		bool isReady() override { return _res; }
		bool sendCommand(const std::string &) override { return _res; }
		bool setCameraPosition(const std::string &) override { return _res; }
		bool setArmPosition(const std::string &) override { return _res; }
		bool rotatePlateStep() override { return _res; }
		bool isStable() override { return _res; }
		std::string getPicoStatus() override { return "OK"; }
};

class mockHttpUploader : public IComputer
{
	public:
		mockHttpUploader() {}
		~mockHttpUploader() = default;
		bool        _res;
		std::string _order;
		bool        isServerReachable() override { return _res; }
		bool        upload(const std::string &) override { return _res; }
		std::string waitForOrder() override { return _order; }
};

extern "C"
{
	#include "unity.h"

	void setUp(void)    {}
	void tearDown(void) {}
}

void test_CheckConnection_success()
{
	mockPicoClient  pico;
	mockHttpUploader uploader;
	pico._res     = true;
	uploader._res = true;
	CheckConnection_UC uc(pico, uploader);
	TEST_ASSERT_EQUAL_INT((int)ConnectionStatus::OK, (int)uc.execute());
}

void test_CheckConnection_pico_not_ready()
{
	mockPicoClient  pico;
	mockHttpUploader uploader;
	pico._res     = false;
	uploader._res = true;
	CheckConnection_UC uc(pico, uploader);
	TEST_ASSERT_EQUAL_INT((int)ConnectionStatus::PICO_UNREACHABLE, (int)uc.execute());
}

void test_CheckConnection_server_unreachable()
{
	mockPicoClient  pico;
	mockHttpUploader uploader;
	pico._res     = true;
	uploader._res = false;
	CheckConnection_UC uc(pico, uploader);
	TEST_ASSERT_EQUAL_INT((int)ConnectionStatus::SERVER_UNREACHABLE, (int)uc.execute());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_CheckConnection_success);
	RUN_TEST(test_CheckConnection_pico_not_ready);
	RUN_TEST(test_CheckConnection_server_unreachable);
	return UNITY_END();
}
