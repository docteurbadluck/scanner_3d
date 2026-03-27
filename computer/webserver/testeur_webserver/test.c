#include "test.h"

void setUp(void) {}
void tearDown(void) {}

int main(void)
{

	UNITY_BEGIN();
    RUN_TEST(test_get_200);
    RUN_TEST(test_post_upload_creates_file);
    RUN_TEST(test_delete_200);
    
	return UNITY_END();
}
