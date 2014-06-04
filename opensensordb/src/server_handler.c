
#include <stdlib.h>
#include "server.h"
#include "osdb.h"

// ----
// Using V8

// GET /scripts/<name>

// ----
// In C

// File system:
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

// HANDLE REQUEST

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

void server_handle_request(request_t* request, response_t* response)
{
        response->status = 200;
        response_printf(response, "Hello web!\n");
        response_content_type(response, "text/plain");
}

