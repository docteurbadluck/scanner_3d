#include "test.h"

#define DELETE_PATH   "computer/webserver/www/upload/delete_ok.txt"
#define DELETE_ROUTE  "DELETE /3D_scanner/upload/delete_ok.txt"

static void create_file_for_deletion(void)
{
    FILE *f = fopen(DELETE_PATH, "w");
    TEST_ASSERT_NOT_NULL_MESSAGE(f, "Could not create file for deletion test");
    fputs("hello", f);
    fclose(f);
}

void test_delete_200(void)
{
    http_response_t res;

    create_file_for_deletion();
    res = make_request_and_parse(DELETE_ROUTE);
    TEST_ASSERT_EQUAL_INT_MESSAGE(200, res.status_code, "Delete should return 200");
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, access(DELETE_PATH, F_OK), "File should no longer exist on disk");
    free(res.headers);
    free(res.body);
}

