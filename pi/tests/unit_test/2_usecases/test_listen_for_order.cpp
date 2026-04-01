#include "2_usecases/ListenForOrder_UC/ListenForOrder_UC.hpp"
#include <string>

class mockHttpUploader : public IHttpUploader
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

void test_ListenForOrder_returns_ping()
{
	mockHttpUploader uploader;
	uploader._order = "PING";
	ListenForOrder_UC uc(uploader);
	TEST_ASSERT_EQUAL_STRING("PING", uc.waitForOrder().c_str());
}

void test_ListenForOrder_returns_empty()
{
	mockHttpUploader uploader;
	uploader._order = "";
	ListenForOrder_UC uc(uploader);
	TEST_ASSERT_EQUAL_STRING("", uc.waitForOrder().c_str());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_ListenForOrder_returns_ping);
	RUN_TEST(test_ListenForOrder_returns_empty);
	return UNITY_END();
}
