
#ifndef _SERVER_H_
#define _SERVER_H_

typedef struct _pathnode_t pathnode_t;

struct _pathnode_t {
        char* name;
        pathnode_t* next;
};

#define REQ_MAX_ARGS 32

enum {
        HTTP_UNKNOWN,
        HTTP_GET
};

typedef struct _request_t {
        int method;
        char* path;
        pathnode_t* pathnodes;
        int num_args;
        char* names[REQ_MAX_ARGS];
        char* values[REQ_MAX_ARGS];
} request_t;

typedef struct _response_t {
        int status;
        char* buf;
        int size;
        int count;
        char content_type[256];
} response_t;

int response_append(response_t* response, char c);
int response_write(response_t* response, char* buffer, int len);

/* print 0-terminated string */
int response_print(response_t* response, char* s);

int response_printf(response_t* response, char* format, ...);

int response_content_type(response_t* response, char* mime_type);

#endif /* _SERVER_H_ */
