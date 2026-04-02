#include "2_usecases/CheckCaptureStorage_UC/CheckCaptureStorage_UC.hpp"

class mockDiskChecker : public IDiskChecker
{
	public:
		mockDiskChecker() {}
		~mockDiskChecker() = default;
		bool _res;
		bool hasEnoughSpace() override { return _res; }
};

extern "C"
{
	#include "unity.h"

	void setUp(void)    {}
	void tearDown(void) {}
}

void test_CheckCaptureStorage_has_space()
{
	mockDiskChecker disk;
	disk._res = true;
	CheckCaptureStorage_UC uc(disk);
	TEST_ASSERT_EQUAL_INT(
		static_cast<int>(CheckCaptureStorageResult::OK),
		static_cast<int>(uc.execute())
	);
}

void test_CheckCaptureStorage_no_space()
{
	mockDiskChecker disk;
	disk._res = false;
	CheckCaptureStorage_UC uc(disk);
	TEST_ASSERT_EQUAL_INT(
		static_cast<int>(CheckCaptureStorageResult::NO_SPACE),
		static_cast<int>(uc.execute())
	);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_CheckCaptureStorage_has_space);
	RUN_TEST(test_CheckCaptureStorage_no_space);
	return UNITY_END();
}
