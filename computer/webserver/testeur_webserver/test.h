#ifndef TEST_H
#define TEST_H

# include "unity.h"
# include "unity_internals.h"
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <errno.h>
# include <fcntl.h>
# include <unistd.h>  
             
typedef struct  
{               
    	int status_code;
    	char *headers;
    	char *body;  
} http_response_t; 

typedef struct http_response_stream_s
{
    int     status;
    char   *body;
    size_t  body_len;
} http_response_stream_t;



http_response_t make_request_and_parse(const char *request_line);
int				http_request(const char *request, char *response, size_t max_len);
http_response_t	parse_http_response(char *raw);
http_response_t post_request(const char *path, const char *body);
http_response_t make_request_and_parse_http_version_1_0(const char *request_line,int keep_alive);

//###GET###
void test_get_200(void);

#endif
