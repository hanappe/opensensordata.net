
#ifndef _DATASTREAM_H
#define _DATASTREAM_H

struct _datastream_t {
        id_t id;
        timestamp_t start;
        timestamp_t end;
        timestamp_t min_period;
        timestamp_t max_period;
        timestamp_t tolerance;
        uint32_t num_timeseries;
        uint32_t num_datapoints;
        value_t sum;
        list_t* timeseries;
        int lock;
};

datastream_t* datastream_create(id_t id,
                                timestamp_t min_period,
                                timestamp_t max_period,
                                timestamp_t tolerance);
datastream_t* datastream_load(id_t id);

void delete_datastream(datastream_t* d);

int datastream_store(datastream_t* d);

int datastream_insert(datastream_t* d, timestamp_t t, value_t v);

int datastream_list(datastream_t* d,
		    timestamp_t start,
		    timestamp_t end,
		    osdb_list_callbacks_t* callbacks);

int datastream_list_timeseries(datastream_t* d, 
                               list_timeseries_callback_t callback, 
                               void* userdata);

int datastream_delete(datastream_t* d, 
                      timestamp_t start,
                      timestamp_t end);

#endif /* _DATASTREAM_H */
