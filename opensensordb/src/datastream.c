#include "osdb_priv.h"

static int datastream_store_fd(datastream_t* d, int fd);

static datastream_t* new_datastream()
{
        datastream_t* d = (datastream_t*) malloc(sizeof(datastream_t));
        if (d == NULL) {
                log_err("new_datastream: out of memory");
                return NULL;
        }
        memset(d, 0, sizeof(datastream_t));

        d->start = 0;
        d->end = 0;
        d->id = 0;
        return d;
}

static void datastream_clear(datastream_t* d)
{
        list_t* l = d->timeseries;
        while (l) {
                delete_timeseries((timeseries_t*) l->data);
                l = l->next;
        }
        delete_list(d->timeseries);
}

void delete_datastream(datastream_t* d)
{
        datastream_clear(d);
        free(d);
}

datastream_t* datastream_create(id_t id,
                                timestamp_t min_period,
                                timestamp_t max_period,
                                timestamp_t tolerance)
{
        datastream_t* d = new_datastream();
        if (d == NULL)
                return NULL;

        d->id = id;
        d->min_period = min_period;
        d->max_period = max_period;
        d->tolerance = tolerance;

        int fd = filemanager_create_datastream(id);
        if (fd == -1) {
                free(d);
                return NULL;
        }
        
        int ret = datastream_store_fd(d, fd);
        close(fd);
        if (ret != 0) {
                free(d);
                return NULL;
        }

        return d;
}

static int _datastream_load(datastream_t* d)
{
        int fd = filemanager_open_datastream(d->id);
        if (fd == -1) 
                return -1;

        if ((fileio_read_uint32(fd, &d->id) != 0)
            || (timestamp_read(&d->start, fd) != 0)
            || (timestamp_read(&d->end, fd) != 0)
            || (timestamp_read(&d->min_period, fd) != 0)
            || (timestamp_read(&d->max_period, fd) != 0)
            || (timestamp_read(&d->tolerance, fd) != 0)
            || (fileio_read_uint32(fd, &d->num_timeseries) != 0)
            || (fileio_read_uint32(fd, &d->num_datapoints) != 0)
            || (value_read(&d->sum, fd) != 0))
                goto error_recovery;

        list_t* prev;

        for (uint32_t i = 0; i < d->num_timeseries; i++) {
                timeseries_t* ts = new_timeseries(i, d->id);
                
                if (timeseries_read_metadata(ts, fd) != 0)
                        goto error_recovery;

                list_t* e = new_list(ts);
                
                if (i == 0) {
                        d->timeseries = e;
                } else {
                        prev->next = e;
                }
                prev = e;
        }

        close(fd);
        return 0;

 error_recovery:
        // FIXME: log | major error!
        log_err("Failed to load the datastream (%d)", d->id);
        close(fd);
        return -1;
}

datastream_t* datastream_load(id_t id)
{
        datastream_t* d = new_datastream(id);
        if (d == NULL) 
                return NULL;

        if (_datastream_load(d) != 0) {
                delete_datastream(d);
                return NULL;
        }

        return d;
}

static int datastream_store_fd(datastream_t* d, int fd)
{
        int err = 0;

        if ((fileio_write_uint32(fd, d->id) != 0)
            || (timestamp_write(d->start, fd) != 0)
            || (timestamp_write(d->end, fd) != 0)
            || (timestamp_write(d->min_period, fd) != 0)
            || (timestamp_write(d->max_period, fd) != 0)
            || (timestamp_write(d->tolerance, fd) != 0)
            || (fileio_write_uint32(fd, d->num_timeseries) != 0)
            || (fileio_write_uint32(fd, d->num_datapoints) != 0)
            || (value_write(d->sum, fd) != 0))
                err++;

        list_t* e = d->timeseries;
        while (e) {
                timeseries_t* ts = (timeseries_t*) e->data;
                if (timeseries_write_metadata(ts, fd) != 0)
                        err++;
                if (timeseries_close(ts) != 0)
                        err++;
                e = e->next;
        }

        if (err != 0) {
                // FIXME: log | major error!
                return -1;
        }

        return 0;
}

int datastream_store(datastream_t* d)
{
        int fd = filemanager_open_datastream(d->id);
        if (fd == -1)
                return -1;
        int ret = datastream_store_fd(d, fd);
        close(fd);
        return ret;
}

static inline void datastream_update(datastream_t* d, timestamp_t t, value_t v)
{
        if (t > d->end)
                d->end = t;
        d->sum += v;
        d->num_datapoints++;
}

static int datastream_insert_new(datastream_t* d, timestamp_t t, value_t v)
{
        timeseries_t* ts = new_timeseries(d->num_timeseries++, d->id);
        if (ts == NULL)
                return -1;
        
        if (timeseries_append(ts, t, v) != 0) 
                return -1;
        
        d->timeseries = list_append(d->timeseries, ts);

        if ((d->start == 0) || (t < d->start))
                d->start = t;
        if ((d->end == 0) || (t > d->end))
                d->end = t;

        datastream_update(d, t, v);
        
        return 0;
}

int datastream_insert(datastream_t* d, timestamp_t t, value_t v)
{
        timestamp_t tmin, tmax;

        list_t* l = d->timeseries;
        while (l) {
                timeseries_t* ts = (timeseries_t*) l->data;

                if (ts->length == 1) {

                        tmin = ts->start + d->min_period;
                        tmax = ts->start + d->max_period;
                        if ((t >= tmin) && (t <= tmax)) {
                                if (timeseries_append(ts, t, v) == 0) {
                                        datastream_update(d, t, v);
                                        return 0;
                                } else return -1;
                        } 

                } else {

                        if ((ts->start <= t) && (t <= ts->end)) {
                                if (timeseries_insert(ts, t, v) == 0) {
                                        datastream_update(d, t, v);
                                        return 0;
                                } else return -1;
                        }

                        tmin = ts->end + (1.0 - d->tolerance) * ts->period;
                        tmax = ts->end + (1.0 + d->tolerance) * ts->period;
                        if ((tmin <= t) && (t <= tmax)) {
                                if (timeseries_append(ts, t, v) == 0) {
                                        datastream_update(d, t, v);
                                        return 0;
                                } else return -1;
                        }
                }

                l = l->next;
        }

        return datastream_insert_new(d, t, v);
}

int datastream_list(datastream_t* d,
		    timestamp_t start,
		    timestamp_t end,
		    osdb_list_callbacks_t* callbacks)
{
        int lock = filemanager_lock_datastream(d->id);
        if (lock < 0)
                return -1;

        list_t* l = d->timeseries;
        while (l) {
                timeseries_t* ts = (timeseries_t*) l->data;
                timeseries_list(ts, start, end, callbacks);
                l = l->next;
        }        

        if (filemanager_unlock_datastream(d->id, lock) != 0)
                return -1;

        return 0;
}

int datastream_list_timeseries(datastream_t* d, 
                               list_timeseries_callback_t callback, 
                               void* userdata)
{
        int lock = filemanager_lock_datastream(d->id);
        if (lock < 0)
                return -1;

        list_t* l = d->timeseries;
        while (l) {
                timeseries_t* ts = (timeseries_t*) l->data;
                (*callback)(userdata, d, ts);
                l = l->next;
        }        

        if (filemanager_unlock_datastream(d->id, lock) != 0)
                return -1;

        return 0;
}

int datastream_delete(datastream_t* d, 
                      timestamp_t start,
                      timestamp_t end)
{
        if (end < start) {
                log_err("datastream_delete: end is smaller than start");
                return -1;
        }

        int lock = filemanager_lock_datastream(d->id);
        if (lock < 0)
                return -1;

        if (filemanager_start_fileops(d->id) != 0) {
                filemanager_unlock_datastream(d->id, lock);
                return -1;
        }

        list_t* l = d->timeseries;
        while (l) {
                timeseries_t* ts = (timeseries_t*) l->data;

                if ((end < ts->start) || (start > ts->end)) {
                        /* do nothing */

                } else if ((start <= ts->start) && (ts->end <= end)) {
                        /* Erase & delete the while timeseries */
                        if (timeseries_erase(ts) == 0)
                                d->timeseries = list_remove_link(d->timeseries, l);

                } else if ((start <= ts->start) 
                           && (ts->start <= end) && (end < ts->end)) {

                        /* Remove a chunk at the beginning */

                        /* Copy tail to a new timeseries */
                        timeseries_t* tail = new_timeseries(d->num_timeseries++, ts->did);
                        if (tail == NULL) 
                                goto error_recovery;

                        int ret = timeseries_copy(ts, end, ts->end, tail);
                        if (ret < 0)
                                goto error_recovery;
                        
                        /* Erase the old timeseries */
                        ret = timeseries_erase(ts);
                        if (ret != 0)
                                goto error_recovery;

                        delete_timeseries(ts);

                        l->data = tail;
                        
                } else if ((ts->start < start) && (start <= ts->end)
                           && (ts->end <= end)) {

                        /* Remove a chunk at the end */
                        timeseries_t* head = new_timeseries(d->num_timeseries++, ts->did);
                        if (head == NULL) 
                                goto error_recovery;
                        
                        int ret = timeseries_copy(ts, ts->start, start, head);
                        if (ret < 0)
                                goto error_recovery;
                        
                        /* Erase the old timeseries */
                        ret = timeseries_erase(ts);
                        if (ret != 0)
                                goto error_recovery;

                        delete_timeseries(ts);

                        l->data = head;
                        
                } else if ((ts->start < start) && (end < ts->end)) {
                        /* Remove a chunk in the middle */

                        timeseries_t* head = new_timeseries(d->num_timeseries++, ts->did);
                        if (head == NULL) 
                                goto error_recovery;
                        
                        int ret = timeseries_copy(ts, ts->start, start, head);
                        if (ret < 0)
                                goto error_recovery;


                        timeseries_t* tail = new_timeseries(d->num_timeseries++, ts->did);
                        if (tail == NULL) 
                                goto error_recovery;

                        ret = timeseries_copy(ts, end, ts->end, tail);
                        if (ret < 0)
                                goto error_recovery;

                        /* Erase the old timeseries */
                        ret = timeseries_erase(ts);
                        if (ret != 0)
                                goto error_recovery;

                        delete_timeseries(ts);

                        l->data = head;
                        
                        list_t* nl = new_list(tail);
                        nl->next = l->next;
                        l->next = nl;
                }

                l = l->next;
        }

        if (datastream_store(d) != 0)
                goto error_recovery;

        if (filemanager_apply_fileops(d->id) != 0)
                goto error_recovery;
        
        if (filemanager_unlock_datastream(d->id, lock) != 0)
                return -1;

        return 0;


 error_recovery:
                        
        filemanager_forget_fileops(d->id);

        datastream_clear(d);
        _datastream_load(d);
        
        filemanager_unlock_datastream(d->id, lock);

        return -1;
}
