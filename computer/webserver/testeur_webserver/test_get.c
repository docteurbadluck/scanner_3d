#include "test.h"


void test_get_200(void)
{
	//TEST_MESSAGE("GET 3D_scanner/ should return 200");

	http_response_t res = make_request_and_parse("GET /3D_scanner/");
	TEST_ASSERT_EQUAL_INT(200, res.status_code);
	res = make_request_and_parse("GET /");  
	TEST_ASSERT_EQUAL_INT(200, res.status_code);

	res = make_request_and_parse("GET /3D_scanner/status.html");
	TEST_ASSERT_EQUAL_INT(200, res.status_code);
	res = make_request_and_parse("GET /3D_scanner/metrics.html");
	TEST_ASSERT_EQUAL_INT(200, res.status_code);
	res = make_request_and_parse("GET /3D_scanner/productivity.html");
	TEST_ASSERT_EQUAL_INT(200, res.status_code);
}

