
#ifndef _FILEIO_H
#define _FILEIO_H

typedef struct _fileio_mmap_t {
        char* data_addr;
        uint32_t data_offset;
        uint32_t data_length;
        char* mmap_addr;
        uint32_t mmap_offset;
        uint32_t mmap_length;
} fileio_mmap_t;

int fileio_read_uint32(int fd, uint32_t* v);
int fileio_write_uint32(int fd, uint32_t v);

int fileio_read_float(int fd, float* v);
int fileio_write_float(int fd, float v);

int fileio_read_double(int fd, double* v);
int fileio_write_double(int fd, double v);

int fileio_mmap(fileio_mmap_t* map, int fd, uint32_t offset, uint32_t length);
int fileio_unmmap(fileio_mmap_t* map);

#endif /* _FILEIO_H */
