
#ifndef _OSD_PRIV_H
#define _OSD_PRIV_H

#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/file.h>
#include "osdb.h"

typedef struct _datapoint_t datapoint_t;
typedef struct _timeseries_t timeseries_t;
typedef struct _datastream_t datastream_t;
typedef struct _list_t list_t;
typedef struct _hashtable_t hashtable_t;

typedef int (*compare_func_t)(void* a, void* b);
typedef void (*list_timeseries_callback_t)(void* userdata, datastream_t* d, timeseries_t* ts);

#include "value.h"
#include "timestamp.h"
#include "filemanager.h"
#include "list.h"
#include "datapoint.h"
#include "timeseries.h"
#include "datastream.h"
#include "fileio.h"
#include "hashtable.h"
#include "log.h"

#endif /* _OSD_PRIV_H */
