#include "2_usecases/SendPhotoToComputer_UC/SendPhotoToComputer_UC.hpp"
#include <string>

class mockCamera : public ICamera
{
	public:
		mockCamera() {}
		~mockCamera() = default;
		bool _res;
		bool        capture() override { return _res; }
		bool        hasPhoto() const override { return _res; }
		std::string getData() const override { return "photo_data"; }
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

void test_SendData_success()
{
	mockCamera      camera;
	mockHttpUploader uploader;
	camera._res   = true;
	uploader._res = true;
	SendPhotoToComputer_UC uc(camera, uploader);
	TEST_ASSERT_TRUE(uc.execute());
}

void test_SendData_no_photo()
{
	mockCamera      camera;
	mockHttpUploader uploader;
	camera._res   = false;
	uploader._res = true;
	SendPhotoToComputer_UC uc(camera, uploader);
	TEST_ASSERT_FALSE(uc.execute());
}

void test_SendData_upload_fails()
{
	mockCamera      camera;
	mockHttpUploader uploader;
	camera._res   = true;
	uploader._res = false;
	SendPhotoToComputer_UC uc(camera, uploader);
	TEST_ASSERT_FALSE(uc.execute());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_SendData_success);
	RUN_TEST(test_SendData_no_photo);
	RUN_TEST(test_SendData_upload_fails);
	return UNITY_END();
}
