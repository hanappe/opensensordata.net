
#include "osdb_priv.h"

int timestamp_read(timestamp_t* t, int fd)
{
        return fileio_read_double(fd, t);
}

int timestamp_write(timestamp_t t, int fd)
{
        return fileio_write_double(fd, t);
}

