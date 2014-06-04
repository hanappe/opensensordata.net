
#include <stdlib.h>
#include "server.h"
#include "osdb.h"

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
}

void server_handle_script(request_t* request, response_t* response)
{
        list_t* l = request->pathnodes;
        l = list_next(l);
        if (l == NULL) {
                response_bad_request(response);
                return;
        }
        const char* account = (const char*) list_get(l);
        
        l = list_next(l);
        if (l == NULL) {
                response_bad_request(response);
                return;
        }
        const char* script = (const char*) l->data;

        if (!request->method == HTTP_GET) {
                response_bad_request(response);
                return;
        }

        /* 

           Execute script and send output to response using
           response_append(), response_write(), response_print(), or
           response_printf(). 

        */
}

void server_handle_request(request_t* request, response_t* response)
{
        list_t* l = request->pathnodes;
        
        if (l == NULL) { // root of web site
                response->status = 200;
                response_printf(response, "Hello web!\n");
                response_content_type(response, "text/plain");
        }

        const char* node = (const char*) l->data;

        if (strcmp(node, "scripts") == 0) {
                server_handle_script(request, response);

        } else if (strcmp(node, "datastream") == 0) {

        }

}

