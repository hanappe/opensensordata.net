#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdarg.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>
#include "log.h"
#include "server.h"

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0

static char* pidFile = "/var/run/opensensordb.pid";
static char* logFile = "/var/log/opensensordb.log";
static int port = 10081;
static int nodaemon = 0;
static int serverSocket = -1;

static pathnode_t* new_pathnode(const char* name)
{
        pathnode_t* node = (pathnode_t*) malloc(sizeof(pathnode_t));
        if (node == NULL) {
                log_err("new_pathnode: Out of memory");
                return NULL;
        }

        node->name = strdup(name);
        if (node->name == NULL) {
                free(node);
                log_err("new_pathnode: Out of memory");
                return NULL;
        } 
        node->next = NULL;
        return node;
}

static void delete_pathnode(pathnode_t* node)
{
        free(node->name);
        free(node);
}

static void delete_pathnodes(pathnode_t* node)
{
        while (node) {
                pathnode_t* next = node->next;
                delete_pathnode(node);
                node = next;
        }
}

int path_parse(const char* path, pathnode_t** nodes)
{
        enum {
                PATH_START,
                PATH_SLASH,
                PATH_NAME
        };
        
        pathnode_t* node = NULL;
        pathnode_t* cur = NULL;
        int state = PATH_START;
        int len = strlen(path);
        char buffer[256];
        int index = 0;

        *nodes = NULL;

        for (int i = 0; i < len; i++) {
                int c = path[i];
                if (c == '/') {
                        if (state == PATH_START) {
                                state = PATH_SLASH;

                        } else if (state == PATH_NAME) {
                                buffer[index] = 0;
                                pathnode_t* n = new_pathnode(buffer);

                                if (node == NULL)
                                        node = cur = n;
                                else {
                                        cur->next = n;
                                        cur = n;
                                }
                                state = PATH_SLASH;
                                        
                        } else if (state == PATH_SLASH) {
                                // state = PATH_SLASH;
                        }

                } else {
                        if (state == PATH_START) {
                                log_warn("Invalid path: %s", path);
                                return -1;

                        } else if (state == PATH_NAME) {
                                if (index < 255) {
                                        buffer[index++] = c;
                                } else {
                                        delete_pathnodes(node);
                                        log_warn("Invalid path: %s", path);
                                        return -1;
                                }

                        } else if (state == PATH_SLASH) {
                                index = 0;
                                buffer[0] = c;
                                state = PATH_NAME;
                        } 
                }
        }

        *nodes = node;

        return 0;
}



static int openServerSocket(int port) 
{
        struct sockaddr_in sockaddr;
        struct in_addr addr;
        int serverSocket = -1;

        int r = inet_aton("127.0.0.1", &addr);
        if (r == 0) {
                log_err("Daemon: Failed to convert '127.0.0.1' to an IP address...?!\n");
                return -1;
        }
        //addr.s_addr = htonl(INADDR_ANY);

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
                log_err("Daemon: Failed to create server socket\n");
                return -1;
        }

        memset((char *)&sockaddr, 0, sizeof(struct sockaddr_in));
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_addr = addr;
        sockaddr.sin_port = htons(port);

        if (bind(serverSocket, (const struct sockaddr *) &sockaddr, 
                 sizeof(struct sockaddr_in)) == -1) {
                close(serverSocket);
                serverSocket = -1;
                log_err("Daemon: Failed to bind server socket\n");
                return -1;
        }
        
        if (listen(serverSocket, 10) == -1) {
                close(serverSocket);
                serverSocket = -1;
                log_err("Daemon: Failed to bind server socket\n");
                return -1;
        }

        return serverSocket;
}

static void closeServerSocket(int serverSocket) 
{
        if (serverSocket != -1) {
                close(serverSocket);
        }
}

static int serverSocketAccept(int serverSocket) 
{
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);

        if (serverSocket == -1) {
                log_err("Daemon: Invalid server socket\n");
                return -1;
        }
  
        int clientSocket = accept(serverSocket, (struct sockaddr*) &addr, &addrlen);
  
        if (clientSocket == -1) {
                log_err("Daemon: Accept failed\n");
                return -1;
        } 

        int flag = 1; 
        setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));

        return clientSocket;
}

static int clientWrite(int clientSocket, char* buffer, int len) 
{
        return send(clientSocket, buffer, len, MSG_NOSIGNAL);
}

static int clientPrint(int clientSocket, char* s) 
{
        int len = strlen(s);
        return clientWrite(clientSocket, s, len);
}

static int clientPrintf(int client, const char* format, ...) 
{
        char buffer[1024];
        va_list ap;

        va_start(ap, format);
        vsnprintf(buffer, 1023, format, ap);
        va_end(ap);
        buffer[1023] = 0;

        return clientPrint(client, buffer);
}

static void closeClient(int clientSocket) 
{
        if (clientSocket != -1) {
                close(clientSocket);
        }
}

void request_clear(request_t* request)
{
        if (request->path) 
                free(request->path);
        if (request->pathnodes) 
                delete_pathnodes(request->pathnodes);
        memset(request, 0, sizeof(request_t));
}

void response_clear(response_t* response)
{
        if (response->buf) 
                free(response->buf);
        memset(response, 0, sizeof(response_t));
}

int response_append(response_t* response, char c)
{
        if (response->count >= response->size) {
                int newsize = 1024 + 2 * response->size;
                response->buf = realloc(response->buf, newsize);
                if (response->buf == NULL) {
                        log_err("Daemon: Out of memory\n");
                        response_clear(response);
                        return -1;
                }
                response->size = newsize;
        }
        response->buf[response->count++] = c;
        return 0;
}

int response_write(response_t* response, char* buffer, int len)
{
        while (--len >= 0) {
                if (response_append(response, *buffer++) != 0)
                        return -1;
        }
        return 0;
}

int response_print(response_t* response, char* s)
{
        int len = strlen(s);
        return response_write(response, s, len);
}

int response_printf(response_t* response, char* format, ...)
{
        char buffer[1024];
        va_list ap;
        int len = sizeof(buffer);

        // FIXME: should use veriable size buffer...
        va_start(ap, format);
        vsnprintf(buffer, len, format, ap);
        buffer[len-1] = 0;
        va_end(ap);

        return response_print(response, buffer);
}

int response_content_type(response_t* response, char* mime_type)
{
        int len = sizeof(response->content_type);
        strncpy(response->content_type, mime_type, len);
        response->content_type[len-1] = 0;
        return 0;
}

int writePidFile(const char* filename) 
{
	pid_t pid = getpid();
        FILE* fp = fopen(filename, "w");
        if (fp) {
                fprintf(fp, "%d\n", pid);
                fclose(fp);
                return 0;
        } else {
                log_err("Daemon: Failed to create the PID file '%s'\n", filename);
        }
        return -1;
}

int removePidFile(const char* filename) 
{
        return unlink(filename);
}

int daemonise() 
{
	switch (fork()) {
        case 0:  
		break;
        case -1: 
		log_err("Daemon: Failed to fork\n");		
		return -1;
        default: 
		_exit(0);
	}
	
	if (setsid() < 0) {
		log_err("Daemon: setsid() failed\n");		
		return -1;
	}

	for (int i = 0; i < 3; ++i) 
		close(i);

	int fd;

	if ((fd = open("/dev/null", O_RDONLY)) == -1) {
		log_err("Daemon: Failed to open /dev/null\n");		
		return -1;
	}

	if ((fd = open("/dev/null", O_WRONLY | O_TRUNC, 0644)) == -1) {
		log_err("Daemon: Failed to open /dev/null\n");		
		return -1;
	}

	if ((fd = open("/dev/null", O_WRONLY | O_TRUNC, 0644)) == -1) {
		log_err("Daemon: Failed to open /dev/null\n");		
		return -1;
	}

	return 0;
}

void sighandler(int signum)
{
	if ((signum == SIGINT) || (signum == SIGHUP) || (signum == SIGTERM)) {
		log_info("Daemon: Caught signal. Shutting down.");
		signal(SIGINT, sighandler);
		signal(SIGHUP, sighandler);
		signal(SIGTERM, sighandler);
		closeServerSocket(serverSocket);
                serverSocket = -1;
	}
}

int signalisation() 
{
	// FIXME: use sigaction(2)
	signal(SIGINT, sighandler);
	signal(SIGHUP, sighandler);
	signal(SIGTERM, sighandler);
	return 0;
}

static void usage(FILE* fp, int argc, char** argv)
{
        fprintf (fp,
                 "Usage: %s [options]\n\n"
                 "Options:\n"
                 "-h | --help          Print this message\n"
                 "-v | --version       Print version\n"
                 "-P | --pidfile       PID file\n"
                 "-L | --logfile       Log file\n"
                 "-p | --port          Port number\n"
                 "-n | --nodaemon      Don't put the applciation in the background\n"
                 "",
                 argv[0]);
}

static void printVersion(FILE* fp)
{
        fprintf (fp,
                 "P2P Food Lab Sensorbox\n"
                 "  Update daemon\n"
                 "  Version %d.%d.%d\n",
                 VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
}

void parseArguments(int argc, char **argv)
{
        static const char short_options [] = "hvnP:L:p:";

        static const struct option
                long_options [] = {
                { "help",       no_argument, NULL, 'h' },
                { "version",    no_argument, NULL, 'v' },
                { "pidfile",    required_argument, NULL, 'P' },
                { "logfile",    required_argument, NULL, 'L' },
                { "port",       required_argument, NULL, 'p' },
                { "nodaemon",   no_argument, NULL, 'n' },
                { 0, 0, 0, 0 }
        };

        for (;;) {
                int index, c = 0;
                
                c = getopt_long(argc, argv, short_options, long_options, &index);

                if (-1 == c)
                        break;

                switch (c) {
                case 0: /* getopt_long() flag */
                        break;

                case 'h':
                        usage(stdout, argc, argv);
                        exit(EXIT_SUCCESS);
                case 'v':
                        printVersion(stdout);
                        exit(EXIT_SUCCESS);
                case 'P':
                        pidFile = optarg;
                        break;
                case 'L':
                        logFile = optarg;
                        break;
                case 'p':
                        port = atoi(optarg);
                        break;
                case 'n':
                        nodaemon = 1;
                        break;

                default:
                        usage(stderr, argc, argv);
                        exit(EXIT_FAILURE);
                }
        }
}

const char* findCommand(const char* request)
{
        typedef struct _command_t {
                char* header;
                char* cmdline;
        } command_t;

        static command_t commands[] = {
                { "/update/network/wifi", "/var/p2pfoodlab/bin/update-network wifi 2>&1" },
                { "/update/network/wired", "/var/p2pfoodlab/bin/update-network wired 2>&1" },
                { "/update/network/gsm", "/var/p2pfoodlab/bin/update-network gsm 2>&1" },
                { "/update/ssh", "/var/p2pfoodlab/bin/update-ssh 2>&1" },
                { "/update/crontab", "/var/p2pfoodlab/bin/update-crontab 2>&1" },
                { "/test/camera", "/var/p2pfoodlab/bin/test-camera 2>&1" },
                { "/update/sensors", "/var/p2pfoodlab/bin/arduino enable-sensors 2>&1" },
                { "/update/version", "/var/p2pfoodlab/bin/update-version 2>&1" },
                { "/update/poweroff", "/var/p2pfoodlab/bin/update-poweroff 2>&1" },
                { NULL, NULL }
        };

        for (int i = 0; commands[i].header != NULL; i++) {
                if (strcmp(request, commands[i].header) == 0) 
                        return commands[i].cmdline;
                
        }

        return NULL;
}

int parseRequest(int client, request_t* req, response_t* resp)
{
        enum {
                REQ_METHOD,
                REQ_PATH,
                REQ_HTTPVERSION,
                REQ_ARGS_NAME,
                REQ_ARGS_VALUE,
                REQ_REQLINE_LF,
                REQ_HEADER_NAME,
                REQ_HEADER_VALUE,
                REQ_HEADER_LF,
                REQ_HEADERS_END_LF,
                REQ_HEADERS_END,
        };

#define BUFLEN 512
        int state = REQ_METHOD;
        char buffer[BUFLEN];
        int count = 0;
        char c;
        int r;

        while (1) {
                
                if (state == REQ_HEADERS_END)
                        return 0;

                r = read(client, &c, 1);
                if ((r == -1) || (r == 0)) {
                        log_warn("Daemon: Failed to parse the request\n");
                        return -1;
                }
                
                switch (state) {

                case REQ_METHOD: 
                        if (c == ' ') {
                                buffer[count] = 0;
                                if (strcmp(buffer, "GET") == 0)
                                        req->method = HTTP_GET;
                                else
                                        req->method = HTTP_UNKNOWN;
                                count = 0;
                                state = REQ_PATH;

                        } else if ((c == '\r') || (c == '\n')) {
                                log_warn("Daemon: Bad request: %s\n", buffer);
                                resp->status = 400;
                                return -1;
                                
                        } else if (count < BUFLEN-1) {
                                buffer[count++] = (char) c;

                        } else {
                                buffer[BUFLEN-1] = 0;
                                log_warn("Daemon: Request line too long: %s\n", buffer);
                                resp->status = 400;
                                return -1;
                        }
                        break;

                case REQ_PATH: 
                        if (c == ' ') {
                                buffer[count++] = 0;
                                req->path = malloc(count);
                                if (req->path == NULL) {
                                        log_err("Out of memory");
                                        return -1;
                                }
                                memcpy(req->path, buffer, count);
                                count = 0;
                                int r = path_parse(req->path, &req->pathnodes);
                                if (r != 0) {
                                        resp->status = 400;
                                        return -1;
                                }
                                state = REQ_HTTPVERSION;

                        } else if (c == '?') {
                                buffer[count++] = 0;
                                req->path = malloc(count);
                                memcpy(req->path, buffer, count);
                                count = 0;
                                state = REQ_ARGS_NAME;

                        } else if ((c == '\r') || (c == '\n')) {
                                log_warn("Daemon: Bad request: %s\n", buffer);
                                resp->status = 400;
                                return -1;

                        } else if (count < BUFLEN-1) {
                                buffer[count++] = (char) c;
                        } else {
                                buffer[BUFLEN-1] = 0;
                                log_warn("Daemon: Requested path too long: %s\n", buffer);
                                resp->status = 400;
                                return -1;
                        }
                        break;

                case REQ_ARGS_NAME: 
                        if (c == ' ') {
                                buffer[count++] = 0;
                                if (req->num_args >= REQ_MAX_ARGS) {
                                        log_warn("Daemon: Too many arguments: %d\n", REQ_MAX_ARGS);
                                        resp->status = 400;
                                        return -1;
                                }
                                req->names[req->num_args++] = strdup(buffer);
                                count = 0;
                                state = REQ_HTTPVERSION;
                                
                        } else if (c == '=') {
                                buffer[count++] = 0;
                                if (req->num_args >= REQ_MAX_ARGS) {
                                        log_warn("Daemon: Too many arguments: %d\n", REQ_MAX_ARGS);
                                        resp->status = 400;
                                        return -1;
                                }
                                req->names[req->num_args] = strdup(buffer);
                                count = 0;
                                state = REQ_ARGS_VALUE;

                        } else if (c == '&') {
                                buffer[count++] = 0;
                                if (req->num_args >= REQ_MAX_ARGS) {
                                        log_warn("Daemon: Too many arguments: %d\n", REQ_MAX_ARGS);
                                        resp->status = 400;
                                        return -1;
                                }
                                req->names[req->num_args++] = strdup(buffer);
                                count = 0;
                                state = REQ_ARGS_NAME;

                        } else if ((c == '\r') || (c == '\n')) {
                                log_warn("Daemon: Bad request: %s\n", buffer);
                                resp->status = 400;
                                return -1;
                        } else if (count < BUFLEN-1) {
                                buffer[count++] = (char) c;
                        } else {
                                buffer[BUFLEN-1] = 0;
                                log_warn("Daemon: Requested path too long: %s\n", buffer);
                                resp->status = 400;
                                return -1;
                        }
                        break;

                case REQ_ARGS_VALUE: 
                        if (c == ' ') {
                                buffer[count++] = 0;
                                req->values[req->num_args++] = strdup(buffer);
                                count = 0;
                                state = REQ_HTTPVERSION;
                                
                        } else if (c == '&') {
                                buffer[count++] = 0;
                                req->values[req->num_args++] = strdup(buffer);
                                count = 0;
                                state = REQ_ARGS_NAME;

                        } else if ((c == '\r') || (c == '\n')) {
                                log_warn("Daemon: Bad request: %s\n", buffer);
                                resp->status = 400;
                                return -1;
                        } else if (count < BUFLEN-1) {
                                buffer[count++] = (char) c;
                        } else {
                                buffer[BUFLEN-1] = 0;
                                log_warn("Daemon: Requested path too long: %s\n", buffer);
                                resp->status = 400;
                                return -1;
                        }
                        break;

                case REQ_HTTPVERSION: 
                        if (c == '\n') {
                                buffer[count] = 0;
                                // check HTTP version? Not needed in this app...
                                count = 0;
                                state = REQ_HEADER_NAME;
                        } else if (c == '\r') {
                                buffer[count] = 0;
                                // check HTTP version? Not needed in this app...
                                count = 0;
                                state = REQ_REQLINE_LF;
                        } else if (strchr("HTTP/1.0", c) == NULL) {
                                log_warn("Daemon: Invalid HTTP version string\n");
                                resp->status = 400;
                                return -1;
                        } else {
                                buffer[count++] = (char) c;
                        }
                        break;

                case REQ_REQLINE_LF: 
                        if (c != '\n') {
                                log_warn("Daemon: Invalid HTTP version string\n");
                                resp->status = 400;
                                return -1;
                        } else {
                                count = 0;
                                state = REQ_HEADER_NAME;
                        }
                        break;

                case REQ_HEADER_NAME: 
                        if ((c == '\n') && (count == 0)) {
                                state = REQ_HEADERS_END;
                                
                        } else if ((c == '\r') && (count == 0)) {
                                state = REQ_HEADERS_END_LF;
                                
                        } else if (c == ':') {
                                buffer[count++] = 0;
                                //log_info("Daemon: Header '%s'\n", buffer);
                                count = 0;
                                state = REQ_HEADER_VALUE;
                        } else if (count < BUFLEN-1) {
                                buffer[count++] = (char) c;
                        } else {
                                buffer[BUFLEN-1] = 0;
                                log_warn("Daemon: Header name too long: %s\n", buffer);
                                resp->status = 400;
                                return -1;
                        }
                        break;

                case REQ_HEADER_VALUE: 
                        if (c == '\r') {
                                buffer[count++] = 0;
                                count = 0;
                                state = REQ_HEADER_LF;
                        } else if (c == '\n') {
                                buffer[count++] = 0;
                                count = 0;
                                state = REQ_HEADER_NAME;
                        } else if (count < BUFLEN-1) {
                                buffer[count++] = (char) c;
                        } else {
                                buffer[BUFLEN-1] = 0;
                                log_warn("Daemon: Header value too long: %s\n", buffer);
                                resp->status = 400;
                                return -1;
                        }
                        break;
 
                case REQ_HEADER_LF: 
                        if (c != '\n') {
                                log_warn("Daemon: Invalid HTTP header\n");
                                resp->status = 400;
                                return -1;
                        } else {
                                count = 0;
                                state = REQ_HEADER_NAME;
                        }
                        break;

                case REQ_HEADERS_END_LF: 
                        if (c != '\n') {
                                log_warn("Daemon: Invalid HTTP header\n");
                                resp->status = 400;
                                return -1;
                        } else {
                                count = 0;
                                state = REQ_HEADERS_END;
                        }
                        break;
                }
       }
}

int main(int argc, char **argv)
{
        int err;

        parseArguments(argc, argv);

        if (nodaemon == 0) {
                err = daemonise();
                if (err != 0) exit(1);

                err = writePidFile(pidFile);
                if (err != 0) exit(1);
        }

        err = signalisation();
        if (err != 0) exit(1);

        serverSocket = openServerSocket(port);
        if (serverSocket == -1) exit(1);

        request_t req;
        response_t resp;

        memset(&req, 0, sizeof(request_t));
        memset(&resp, 0, sizeof(response_t));
                        
        while (serverSocket != -1) {

                int client = serverSocketAccept(serverSocket);
                if (client == -1)
                        continue;
                
                int r = parseRequest(client, &req, &resp);
                if (r != 0) {
                        clientPrintf(client, "HTTP/1.1 %03d\r\n", resp.status);
                        closeClient(client);
                        continue;
                }

                if (1) {
                        printf("path: %s\n", req.path);
                        for (int i =0; i < req.num_args; i++)
                                printf("arg[%d]: %s = %s\n", i, req.names[i], req.values[i]);
                }


                // HANDLE REQUEST
                resp.status = 200;
                response_printf(&resp, "Hello web!\n");
                response_content_type(&resp, "text/plain");
                // HANDLE REQUEST

                clientPrintf(client, 
                             "HTTP/1.1 %03d\r\n"
                             "Content-Length: %d\r\n"
                             "Content-Type: %s\r\n"
                             "\r\n", 
                             resp.status, resp.count, resp.content_type);
                clientWrite(client, resp.buf, resp.count);

                closeClient(client);
                
                request_clear(&req);
                response_clear(&resp);
        }

        removePidFile(pidFile); 

        return 0;
}
