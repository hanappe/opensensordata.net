
#ifndef _DATAPOINT_H
#define _DATAPOINT_H

struct _datapoint_t {
        timestamp_t t;
        value_t v;
};

datapoint_t* datapoint_init(datapoint_t* p, timestamp_t t, value_t v);
datapoint_t* datapoint_create(timestamp_t t, value_t v);
int datapoint_read(datapoint_t* p, int fd);
int datapoint_write(datapoint_t* p, int fd);

#endif /* _DATAPOINT_H */
