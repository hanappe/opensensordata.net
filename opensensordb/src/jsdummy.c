#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "osdb.h"

char* server_execute_script(const char* code, list_t* args)
{
        return strdup("{ \"errno\": 1, \"message\": \"Not yet implemented\" }");
}

void server_free_result(char* buffer)
{
        free(buffer);
}
