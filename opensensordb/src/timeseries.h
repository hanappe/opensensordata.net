
#ifndef _TIMESERIES_H
#define _TIMESERIES_H

typedef struct _timeseries_t {
        id_t id;
        id_t did;
        timestamp_t start;
        timestamp_t end;
        timestamp_t period;
        uint32_t length;
        int fd;
} timeseries_t;

timeseries_t* new_timeseries(id_t id, id_t did);
void delete_timeseries(timeseries_t* ts);

int timeseries_close(timeseries_t* ts);
int timeseries_erase(timeseries_t* ts);

int timeseries_read_metadata(timeseries_t* ts, int fd);
int timeseries_write_metadata(timeseries_t* ts, int fd);

int timeseries_append(timeseries_t* ts, timestamp_t t, value_t v);
int timeseries_insert(timeseries_t* ts, timestamp_t t, value_t v);

int32_t timeseries_list(timeseries_t* ts, 
                        timestamp_t start, 
                        timestamp_t end,
                        osdb_list_callbacks_t* callbacks);

int timeseries_copy(timeseries_t* ts, timestamp_t from, timestamp_t to, timeseries_t* dest);

int timeseries_truncate(timeseries_t* ts, timestamp_t end);

#endif /* _TIMESERIES_H */
