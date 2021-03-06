
#ifndef _SERVER_H_
#define _SERVER_H_

#include "list.h"


#ifdef __cplusplus
extern "C" {
#endif

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
        char* body;
        int length;
        int buflen;
} request_t;

typedef struct _response_t {
        int status;
        char* body;
        int length;
        int buflen;
        char content_type[256];
        int mybuf;
        void (*freebuf)(char*);
} response_t;

int response_append(response_t* response, char c);
int response_write(response_t* response, const char* buffer, int len);
int response_print(response_t* response, const char* s); /* print 0-terminated string */
int response_printf(response_t* response, const char* format, ...);
int response_content_type(response_t* response, const char* mime_type);
void response_clear(response_t* response);

int server_init();
int server_set_option(const char* name, const char* value);
void server_handle_request(request_t* request, response_t* response);

#ifdef __cplusplus
}
#endif

#endif /* _SERVER_H_ */
