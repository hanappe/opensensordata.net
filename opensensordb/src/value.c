
#include "osdb_priv.h"

int value_read(value_t* v, int fd)
{
        return fileio_read_double(fd, v);
}

int value_write(value_t v, int fd)
{
        return fileio_write_double(fd, v);
}
