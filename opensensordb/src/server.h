
#ifndef _SERVER_H_
#define _SERVER_H_

#include "list.h"

#define REQ_MAX_ARGS 32

enum {
        HTTP_UNKNOWN,
        HTTP_GET,
        HTTP_PUT,
        HTTP_POST
};

typedef struct _pair_t {
        char* name;
        char* value;
} pair_t;

typedef struct _request_t {
        int method;
        char* path;
        list_t* pathnodes;
        list_t* args;
        list_t* headers;
        char* buf;
        int size;
        int count;
} request_t;

typedef struct _response_t {
        int status;
        int mybuf;
        char* buf;
        int size;
        int count;
        char content_type[256];
} response_t;

int response_append(response_t* response, char c);
int response_write(response_t* response, char* buffer, int len);
int response_print(response_t* response, char* s); /* print 0-terminated string */
int response_printf(response_t* response, char* format, ...);
int response_content_type(response_t* response, char* mime_type);

int server_init();
int server_set_option(const char* name, const char* value);
void server_handle_request(request_t* request, response_t* response);

#endif /* _SERVER_H_ */
