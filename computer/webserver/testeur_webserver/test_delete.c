#include "test.h"

void test_delete_200(void)
{
    TEST_MESSAGE("DELETE /3D_scanner/upload/delete_ok.txt should return 200");
    system("echo hello > /home/docteurbadluck/Desktop/3D_scanner/computer/webserver/www/upload/delete_ok.txt");
    http_response_t res =
        make_request_and_parse("DELETE /3D_scanner/upload/delete_ok.txt");
    TEST_ASSERT_EQUAL_INT(200, res.status_code);
    TEST_ASSERT_EQUAL_INT(-1, access("3D_scanner/upload/delete_ok.txt", F_OK));
}

