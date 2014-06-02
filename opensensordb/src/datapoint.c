 
#include "osdb_priv.h"

datapoint_t* datapoint_init(datapoint_t* p, timestamp_t t, value_t v)
{
        p->t = t;
        p->v = v;
        return p;
}

datapoint_t* datapoint_create(timestamp_t t, value_t v)
{
        datapoint_t* p = (datapoint_t*) malloc(sizeof(datapoint_t));
        if (p == NULL) {
                log_err("datapoint_create: out of memory");
                return NULL;
        }
        p->t = t;
        p->v = v;
        return p;
}

int datapoint_read(datapoint_t* p, int fd)
{
        if ((timestamp_read(&p->t, fd) != 0) 
            || (value_read(&p->v, fd) != 0))
                return -1;
    
        return 0;
}

int datapoint_write(datapoint_t* p, int fd)
{
        if ((timestamp_write(p->t, fd) != 0) 
            || (value_write(p->v, fd) != 0)) 
                return -1;

        return 0;
}


