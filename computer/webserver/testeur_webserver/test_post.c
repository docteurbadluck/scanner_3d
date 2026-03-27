#include "test.h"

#define UPLOAD_ROUTE  "/3D_scanner/upload/test_upload.txt"
#define UPLOAD_PATH   "computer/webserver/www/upload/test_upload.txt"
#define UPLOAD_BODY   "hello upload"

static void verify_file_content(void)
{
    FILE *f = fopen(UPLOAD_PATH, "r");
    char buf[64];
    size_t n;

    TEST_ASSERT_NOT_NULL_MESSAGE(f, "Cannot open uploaded file");
    n = fread(buf, 1, sizeof(buf) - 1, f);
    buf[n] = '\0';
    fclose(f);
    TEST_ASSERT_EQUAL_STRING(UPLOAD_BODY, buf);
}

void test_post_upload_creates_file(void)
{
    http_response_t res = post_request(UPLOAD_ROUTE, UPLOAD_BODY);

    TEST_ASSERT_EQUAL_INT_MESSAGE(200, res.status_code, "Upload should return 200");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, access(UPLOAD_PATH, F_OK), "Uploaded file should exist on disk");
    verify_file_content();
    remove(UPLOAD_PATH);
    free(res.headers);
    free(res.body);
}