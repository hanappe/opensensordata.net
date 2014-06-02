
#include "osdb_priv.h"

struct _osdb_t
{
        hashtable_t* cache;
};

static void delete_datastream_callback(datastream_t* d)
{
        delete_datastream(d);
}

osdb_t* osdb_init()
{        
        osdb_t* db = (osdb_t*) malloc(sizeof(osdb_t));
        if (db == NULL) {
                log_err("osdb_init: out of memory");
                return NULL;
        }
        memset(db, 0, sizeof(osdb_t));

        db->cache = new_hashtable((hashtable_delete_t) delete_datastream_callback);
        if (db->cache == NULL) {
                free(db);
                return NULL;
        }

        return db;
}

int osdb_set_option(osdb_t* osdb, const char* option, const char* value)
{
        if (strcmp(option, "dir") == 0)
                return filemanager_init(value);
        return -1;
}

int osdb_finalize(osdb_t* db)
{
        free(db);
        return 0;
}

static datastream_t* osdb_cache_get(osdb_t* osdb, id_t id)
{
        void* value = NULL;
        hashtable_lookup(osdb->cache, id, &value);
        return (datastream_t*) value;
}

static void osdb_cache_put(osdb_t* osdb, datastream_t* d)
{
        hashtable_insert(osdb->cache, d->id, d);
}

int osdb_create_datastream(osdb_t* db, 
                           id_t id,
                           timestamp_t min_period,
                           timestamp_t max_period,
                           timestamp_t tolerance)
{
        if (filemanager_datastream_exists(id))
                return 0;
        datastream_t* d = datastream_create(id, min_period, max_period, tolerance);
        if (d == NULL)
                return -1;
        osdb_cache_put(db, d);
        return 0;
}

int osdb_datastream_insert(osdb_t* db, id_t id, timestamp_t t, value_t v)
{
        datastream_t* d = osdb_cache_get(db, id);
        if (d == NULL) {
                d = datastream_load(id);
                if (d == NULL)
                        return -1;
                osdb_cache_put(db, d);
        }
        
        return datastream_insert(d, t, v);
}

int osdb_datastream_list(osdb_t* db, 
                        id_t id,
                        timestamp_t start,
                        timestamp_t end,
                        osdb_list_callbacks_t* callbacks)
{
        datastream_t* d = osdb_cache_get(db, id);
        if (d == NULL) {
                d = datastream_load(id);
                if (d == NULL)
                        return -1;
                osdb_cache_put(db, d);
        }
        
        return datastream_list(d, start, end, callbacks);
}

int osdb_datastream_start(osdb_t* db, id_t id, timestamp_t* t)
{
        datastream_t* d = osdb_cache_get(db, id);
        if (d == NULL) {
                d = datastream_load(id);
                if (d == NULL)
                        return -1;
                osdb_cache_put(db, d);
        }

        *t = d->start;
        return 0;
}

int osdb_datastream_end(osdb_t* db, id_t id, timestamp_t* t)
{
        datastream_t* d = osdb_cache_get(db, id);
        if (d == NULL) {
                d = datastream_load(id);
                if (d == NULL)
                        return -1;
                osdb_cache_put(db, d);
        }

        *t = d->end;
        return 0;
}

int osdb_datastream_average(osdb_t* db, id_t id, value_t* v)
{
        datastream_t* d = osdb_cache_get(db, id);
        if (d == NULL) {
                d = datastream_load(id);
                if (d == NULL)
                        return -1;
                osdb_cache_put(db, d);
        }

        *v = (d->sum / (value_t) d->num_datapoints);
        return 0;        
}

int osdb_datastream_count(osdb_t* db, id_t id, unsigned long int* n)
{
        datastream_t* d = osdb_cache_get(db, id);
        if (d == NULL) {
                d = datastream_load(id);
                if (d == NULL)
                        return -1;
                osdb_cache_put(db, d);
        }

        *n = d->num_datapoints;
        return 0;
}

int osdb_datastream_delete(osdb_t* db, 
                           id_t id,
                           timestamp_t start,
                           timestamp_t end)
{
        datastream_t* d = osdb_cache_get(db, id);
        if (d == NULL) {
                d = datastream_load(id);
                if (d == NULL)
                        return -1;
                osdb_cache_put(db, d);
        }
        
        return datastream_delete(d, start, end);
}


