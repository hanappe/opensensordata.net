#include <sys/mman.h>
#include "osdb_priv.h"

int fileio_read_uint32(int fd, uint32_t* v)
{
        size_t n = read(fd, (void*) v, sizeof(uint32_t));
        if (n != sizeof(uint32_t))
                return -1;
        return 0;
}

int fileio_write_uint32(int fd, uint32_t v)
{
        size_t n = write(fd, (void*) &v, sizeof(uint32_t));
        if (n != sizeof(uint32_t))
                return -1;
        return 0;
}

int fileio_read_float(int fd, float* v)
{
        size_t n = read(fd, (void*) v, sizeof(float));
        if (n != sizeof(float))
                return -1;
        return 0;
}

int fileio_write_float(int fd, float v)
{
        size_t n = write(fd, (void*) &v, sizeof(float));
        if (n != sizeof(float))
                return -1;
        return 0;
}

int fileio_read_double(int fd, double* v)
{
        size_t n = read(fd, (void*) v, sizeof(double));
        if (n != sizeof(double))
                return -1;
        return 0;
}

int fileio_write_double(int fd, double v)
{
        size_t n = write(fd, (void*) &v, sizeof(double));
        if (n != sizeof(double))
                return -1;
        return 0;
}

int fileio_mmap(fileio_mmap_t* map, int fd, uint32_t offset, uint32_t length)
{
           struct stat sb;

           if (fstat(fd, &sb) == -1) {
                   log_err("mmap failed: file not found");
                   return -1;
           }
           if (offset >= sb.st_size) {
                   log_err("mmap failed: offset > size");
                   return -1;
           }
           if (offset + length > sb.st_size) {
                   log_err("mmap failed: offset + length > size");
                   return -1;
           }

           map->mmap_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
           map->mmap_length = length + offset - map->mmap_offset;
           map->mmap_addr = mmap(NULL, length + offset - map->mmap_offset, PROT_READ,
                                 MAP_PRIVATE, fd, map->mmap_offset);
           if (map->mmap_addr == MAP_FAILED) {
                   /* char buf[512]; */
                   /* log_err("mmap failed: %s", strerror_r(errno, buf, sizeof(buf))); */
                   log_err("mmap failed: %s", strerror(errno));
                   return -1; 
           }

           map->data_addr = map->mmap_addr + offset - map->mmap_offset;
           map->data_offset = offset;
           map->data_length = length;

           return 0;
}

int fileio_unmmap(fileio_mmap_t* map)
{
        return munmap(map->mmap_addr, map->mmap_length);
}
