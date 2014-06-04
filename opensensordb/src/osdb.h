
#ifndef _OSDB_H
#define _OSDB_H

#ifdef _cplusplus
#extern "C" {
#endif

#include <stdint.h> 

typedef uint32_t id_t;
typedef double timestamp_t;
typedef double value_t;

typedef int (*osdb_start_callback_t)(void* userdata, id_t did, uint32_t count);
typedef int (*osdb_data_callback_t)(void* userdata, id_t did, timestamp_t t, timestamp_t t_orig, value_t v);
typedef int (*osdb_end_callback_t)(void* userdata, id_t did, uint32_t count);

typedef struct _osdb_list_callbacks_t {
        void* userdata;
        osdb_start_callback_t start;
        osdb_data_callback_t data;
        osdb_end_callback_t end;
} osdb_list_callbacks_t;


typedef struct _osdb_t osdb_t;

osdb_t* osdb_init();

/*
  Available options:
  - "dir": Set root directory.
 */
int osdb_set_option(osdb_t* osdb, const char* option, const char* value);
int osdb_finalize(osdb_t* db);

int osdb_create_datastream(osdb_t* db, 
                           id_t id,
                           timestamp_t min_period,
                           timestamp_t max_period,
                           timestamp_t tolerance);

int osdb_datastream_insert(osdb_t* db, id_t id, timestamp_t t, value_t v);

int osdb_datastream_start(osdb_t* db, id_t id, timestamp_t* t);
int osdb_datastream_end(osdb_t* db, id_t id, timestamp_t* t);
int osdb_datastream_average(osdb_t* db, id_t id, value_t* v);
int osdb_datastream_count(osdb_t* db, id_t id, unsigned long int* n);

int osdb_datastream_list(osdb_t* db, 
                        id_t id,
                        timestamp_t start,
                        timestamp_t end,
                        osdb_list_callbacks_t* callbacks);

int osdb_datastream_delete(osdb_t* db, 
                           id_t id,
                           timestamp_t start,
                           timestamp_t end);

#ifdef _cplusplus
}
#endif

#endif /* _OSDB_H */
