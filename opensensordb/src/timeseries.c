#include "osdb_priv.h"

static inline uint32_t timeseries_lower_index(timeseries_t* ts, timestamp_t t)
{
        return (uint32_t) floor((t - ts->start) / ts->period);
}

static inline uint32_t timeseries_upper_index(timeseries_t* ts, timestamp_t t)
{
        return (uint32_t) ceil((t - ts->start) / ts->period);
}

static inline int timeseries_lock(timeseries_t* ts)
{
        return 0;
}

static inline int timeseries_unlock(timeseries_t* ts)
{
        return 0;
}

static int timeseries_open(timeseries_t* ts)
{
        if (ts->fd != -1)
                return 0;

        ts->fd = filemanager_open_timeseries(ts->did, ts->id);
        if (ts->fd == -1)
                return -1;
        
        return 0;
}

int timeseries_close(timeseries_t* ts)
{
        if (ts->fd == -1)
                return 0;
        int ret = filemanager_close_timeseries(ts->did, ts->id, ts->fd);
        ts->fd = -1;
        return ret;
}

timeseries_t* new_timeseries(id_t id, id_t did)
{
        timeseries_t* ts = (timeseries_t*) malloc(sizeof(timeseries_t));
        if (ts == NULL) {
                log_err("new_timeseries: out of memory");
                return NULL;
        }

        memset(ts, 0, sizeof(timeseries_t));
        ts->id = id;
        ts->did = did;
        ts->fd = -1;
        return ts;
}

void delete_timeseries(timeseries_t* ts)
{
        free(ts);
}

int timeseries_erase(timeseries_t* ts)
{
        if (timeseries_close(ts) != 0)
                return -1;
        return filemanager_unlink_timeseries(ts->did, ts->id);
}

int timeseries_read_metadata(timeseries_t* ts, int fd)
{
        if ((fileio_read_uint32(fd, &ts->id) != 0)
            || (fileio_read_uint32(fd, &ts->did) != 0)
            || (timestamp_read(&ts->start, fd) != 0)
            || (timestamp_read(&ts->end, fd) != 0)
            || (timestamp_read(&ts->period, fd) != 0)
            || (fileio_read_uint32(fd, &ts->length) != 0))
                return -1;
        return 0;
}

int timeseries_write_metadata(timeseries_t* ts, int fd)
{
        if ((fileio_write_uint32(fd, ts->id) != 0)
            || (fileio_write_uint32(fd, ts->did) != 0)
            || (timestamp_write(ts->start, fd) != 0)
            || (timestamp_write(ts->end, fd) != 0)
            || (timestamp_write(ts->period, fd) != 0)
            || (fileio_write_uint32(fd, ts->length) != 0))
                return -1;
        return 0;
}

int32_t timeseries_list(timeseries_t* ts, 
                        timestamp_t start, 
                        timestamp_t end,
                        osdb_list_callbacks_t* callbacks)
{
        timestamp_t ts_end = timestamp_nmul(ts->start, ts->length - 1, ts->period);

        if ((end < ts->start) || (start > ts_end) || (end < start))
                return 0;

        int start_index = 0;
        int end_index = ts->length - 1;
        int ret;

        if (start > ts->start) {
                start_index = (start + ts->period - 1 - ts->start) / ts->period;
        }
        if (end < ts_end) {
                end_index = (end + ts->period - 1 - ts->start) / ts->period;
        }

        if (callbacks->start) {
                ret = (*callbacks->start)(callbacks->userdata, ts->did, end_index - start_index);
                if (ret != 0) 
                        return ret;
        }

        if (callbacks->data) {
                fileio_mmap_t map;
                
                if (timeseries_open(ts) == -1) 
                        return -1;
                
                int err = fileio_mmap(&map, ts->fd, start_index * sizeof(datapoint_t), 
                                      (end_index - start_index) * sizeof(datapoint_t));
                if (err != 0) {
                        log_err("mmap failed for timeseries (%d,%d)", ts->did, ts->id);
                        timeseries_close(ts);
                        return -1;
                }
                
                timestamp_t t = ts->start + start_index * ts->period;
                datapoint_t* p = (datapoint_t*) map.data_addr;
                
                for (int i = start_index; i < end_index; i++, p++, t += ts->period) {
                        ret = (*callbacks->data)(callbacks->userdata, ts->did, t, p->t, p->v);
                        if (ret != 0) {
                                fileio_unmmap(&map);
                                timeseries_close(ts);
                                return ret;
                        }
                }
        
                fileio_unmmap(&map);
                
                if (timeseries_close(ts) != 0)
                        return -1;
        }

        if (callbacks->end) {
                ret = (*callbacks->end)(callbacks->userdata, ts->did, end_index - start_index);
                if (ret != 0) 
                        return ret;
        }

        return (int32_t) end_index - start_index;
}

static inline int _write_datapoint(int fd, timestamp_t t, value_t v)
{
        if ((timestamp_write(t, fd) != 0) 
            || (value_write(v, fd) != 0)) 
                return -1;
        return 0;
}

int timeseries_append(timeseries_t* ts, timestamp_t t, value_t v)
{
        if (timeseries_open(ts) != 0)
                return -1;
        if (lseek(ts->fd, 0, SEEK_END) == (off_t) -1) {
                // FIXME: log, check errno
                log_err("fseek() failed in timeseries file (%d, %d): %s", 
                        ts->did, ts->id, strerror(errno));
                return -1;
        }
        if (_write_datapoint(ts->fd, t, v) != 0) {
                return -1;
        }

        timestamp_t interval = t - ts->end;

        ts->length++;
        ts->end = t;
        
        if (ts->length == 1) {
                ts->start = t;

        } else if (ts->length == 2) {
                ts->period = interval;

        } else {
                ts->period = (ts->period * (ts->length - 2) + interval) / (ts->length - 1);
        }

        return 0;
}

int timeseries_insert(timeseries_t* ts, timestamp_t t, value_t v)
{
        uint32_t n = timeseries_lower_index(ts, t);
        double alpha = (t - n * ts->period) / ts->period;
        uint32_t index;

        if (alpha < 0.1) {
                index = (uint32_t) n;
        } else if (alpha > 0.9) {
                index = (uint32_t) n + 1;
        } else {
                return -1;
        }

        if (timeseries_open(ts) != 0)
                return -1;

        if (lseek(ts->fd, index * sizeof(datapoint_t), SEEK_SET) == (off_t) -1) {
                // FIXME: log, check errno
                return -1;
        }
        if (_write_datapoint(ts->fd, t, v) != 0) {
                return -1;
        }

        return 0;
}

int timeseries_truncate(timeseries_t* ts, timestamp_t t)
{
        uint32_t index = timeseries_upper_index(ts, t);
        uint32_t length = index * sizeof(datapoint_t);
        
        int ret = timeseries_open(ts);
        if (ret != 0)
                return -1;  
                
        ret = ftruncate(ts->fd, length);
        if (ret != 0)
                return -1;

        ts->end = ts->start + index * ts->period;

        return 0;
}

int copy_data_callback(void* userdata, id_t did, timestamp_t t, timestamp_t t_orig, value_t v)
{
        if (timeseries_append((timeseries_t*) userdata, t, v) != 0) {
                return -1;
        }
        return 0;
}

int timeseries_copy(timeseries_t* ts, timestamp_t from, timestamp_t to, timeseries_t* dest)
{
        osdb_list_callbacks_t callbacks = { dest, NULL, copy_data_callback, NULL};
        return timeseries_list(ts, from, to, &callbacks);
}
