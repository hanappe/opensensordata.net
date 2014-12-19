
#include <stdlib.h>

#include "server.h"
#include "osdb_priv.h"

#include <iostream>


#include "jsv8.h"

//char* server_execute_script(const char* code, list_t* args, char** content_type);
//void server_free_result(char* buffer);

// ----
// Using V8

// GET /scripts/<account_name>/<script_name>.js



// ----
// In C

// Through file system:
// GET /datastream/<id>.json
// GET /group/<id>.json


// PUT /datastream/<id>.json          -> create/overwrite the object
// POST /datastream/<id>.json         -> update the object

// PUT /group/<id>.json               -> create/overwrite the object
// POST /group/<id>.json              -> update the object


// GET /datapoints/<id>.{json,csv}
// GET /datapoints/<id>/<from>.{json,csv}
// GET /datapoints/<id>/<from>/<to>.{json,csv}


// POST /datapoints.csv                -> upload data in CSV format
// POST /datapoints.json               -> upload data in JSON format


// ----
// Later...

// GET /photostream/<id>.json          
// PUT /photostream/<id>.json          -> create/overwrite the object
// POST /photostream/<id>.json         -> update the object

// GET /photo/<id>/<date>_<size>.jpg   -> get a photo
// PUT /photo/<id>/<date>.jpg          -> upload a photo


static osdb_t* osdb;

int server_init()
{
        osdb = osdb_init();
        if (osdb == NULL)
                return -1;

        return 0;
}

int server_set_option(const char* name, const char* value)
{
        return osdb_set_option(osdb, name, value);
}

void response_error(response_t* response, int status)
{
        response_clear(response);
        response->status = status;


}

void response_bad_request(response_t* response)
{
        response_error(response, 400);

        response_content_type(response, " text/html");

        const char* body = "Bad Request\r\n"; 
        response->body = strdup(body);
        response->length = strlen(body);
        response->mybuf = 1;
        response->freebuf = server_free_result;
}

void response_not_found(response_t* response)
{
        response_error(response, 404);

        response_content_type(response, " text/html");

        const char* body = "Not Found\r\n"; 
        response->body = strdup(body);
        response->length = strlen(body);
        response->mybuf = 1;
        response->freebuf = server_free_result;
}

void response_internal_error(response_t* response)
{
        response_error(response, 500);

        response_content_type(response, " text/html");

        const char* body = "Internall Error\r\n"; 
        response->body = strdup(body);
        response->length = strlen(body);
        response->mybuf = 1;
        response->freebuf = server_free_result;
}

void server_handle_script(request_t* request, response_t* response)
{
        list_t* l = request->pathnodes;
        l = list_next(l);
        if (l == NULL) {
                fprintf(stdout, "Debug bad a\n");
                response_bad_request(response);
                return;
        }
 
        const char* account = (const char*) list_get(l);

        l = list_next(l);
        if (l == NULL) {
                fprintf(stdout, "Debug bad b\n");
                response_bad_request(response);
                return;
        }

        const char* script = (const char*) l->data;

        if (!request->method == HTTP_GET) {
                fprintf(stdout, "Debug bad c\n");
                response_bad_request(response);
                return;
        }

        char* code;
        int r = filemanager_get_script(account, script, &code);        

        if (r == -1) {
                response_bad_request(response);
                return;
        } else if (r == -2) {
                response_not_found(response);
                return;
        } else if (r != 0) {
                response_internal_error(response);
                return;
        }

        log_info("Executing script: %s", script);

        char* content_type = 0;
        char* result = server_execute_script(code, request->args, &content_type);

        filemanager_release_script(code);

        int len = result ? strlen(result) : 0;

        // Fill response

        {
                response->status = 200;

                // Kev: handle content type, quick and dirty...
                
                if (content_type) {
                        response_content_type(response, content_type);
                        free(content_type);
                } else {
                        response_content_type(response, "application/json");
                }
                
                response->body = result;
                response->length = len;
                response->mybuf = 1;
                response->freebuf = server_free_result;
        }
}

void server_handle_request(request_t* request, response_t* response)
{

        list_t* l = request->pathnodes;
        
        if (l == NULL) { // root of web site
                response->status = 200;
                response_printf(response, "Hello web!\n");
                response_content_type(response, "text/plain");
                return;
        }

        const char* node = (const char*) l->data;


        fprintf(stderr, "server_handler.c node: %s\n", node);
        if (strcmp(node, "scripts") == 0) {

                server_handle_script(request, response);

        } else if (strcmp(node, "datastream") == 0) {

        } else {
                response_bad_request(response);
        }

}

