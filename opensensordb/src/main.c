
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <getopt.h>
#include "osdb_priv.h"

static const char* _dir = "/tmp";

static int start_callback(void* userdata, id_t id, uint32_t count)
{
        printf("[\n");
        return 0;
}

static int data_callback(void* userdata, id_t id, timestamp_t t, timestamp_t t_orig, value_t v)
{
        printf("[%d,%f,%f]\n", id, t, v);
        return 0;
}

static int end_callback(void* userdata, id_t id, uint32_t count)
{
        printf("]\n");
        return 0;
}

static int store_datastreams_callback(id_t id, void* value, void* data)
{
        datastream_t* d = (datastream_t*) value;
        if (datastream_store(d) != 0) {
                // FIXME: major problem.
        }
        return 0;
}

static void print_usage() 
{
        fprintf(stderr, "Usage: opensensordb [options] command\n");
        fprintf(stderr, "Try -h for help.\n");
        exit(0);
}

#define VERSION "DEBUG-0.0.1"

static void print_version()
{
  printf("opensensordb %s\n", VERSION);
  exit(0);
}

static void print_help() 
{
        printf("Usage: \n");
        printf("  opensensordb [options] command [command-options]\n");
        printf("Commands:\n");
        printf(" timeseries datastreams-id\n"
               "    List available timeseries of a datastream\n\n");
        printf(" import path\n"
               "    Import a CSV file\n\n");
        printf(" select datastream start end\n"
               "    Print the datapoints between start and end\n\n");
        printf(" delete datastream start end\n"
               "    Delete the datapoints between start and end\n\n");
        printf("Options:\n");
        printf(" -d, --dir=[path]\n"
               "    The path to where the database files are stored [default = /tmp]\n\n");
        printf(" -h, --help\n"
               "    Print out this help summary\n\n");
        printf(" -v, --version\n"
               "    Show version of program\n\n");
        exit(0);
}

static int import_csv(const char* path) 
{
        FILE* fp = fopen(path, "r");
        if (fp == NULL) {
                fprintf(stderr, "File not found: %s\n", path);
                return -1;
        }

        hashtable_t* datastreams = new_hashtable(NULL);

        while (1) {
                unsigned long id;
                double t, v;

                int n = fscanf(fp, "%lu,%lf,%lf\n", &id, &t, &v);
                if (n < 3)
                        break;

                datastream_t* d = NULL;
                hashtable_lookup(datastreams, id, (void**) &d);
                
                if (d == NULL) {
                        d = datastream_load(id);
                        if (d == NULL) {
                                d = datastream_create(id, 60.0, 3600.0, 0.1);
                                if (d == NULL) {
                                        fprintf(stderr, "Failed to create the datastream\n");
                                        goto error_recovery;
                                }
                        }
                        hashtable_insert(datastreams, id, d);
                }

                printf("id=%d, t=%d, v=%f\n", (int) id, (int) t, v);

                int err = datastream_insert(d, (timestamp_t) t, (value_t) v);
                if (err != 0)
                        goto error_recovery;
        }

        fclose(fp);
        hashtable_foreach(datastreams, store_datastreams_callback, NULL);
        return 0;

 error_recovery:
        fclose(fp);
        hashtable_foreach(datastreams, store_datastreams_callback, NULL);
        return -1;
}

void list_timeseries_callback(void* userdata, datastream_t* d, timeseries_t* ts)
{
        printf("Timeseries %lu, Start %f, End %f, Period %f, Length %lu\n",
               (unsigned long) ts->id, 
               ts->start, ts->end, ts->period, 
               (unsigned long) ts->length);
}

static int list_timeseries(id_t id) 
{
        datastream_t* d = datastream_load(id);
        if (d == NULL) {
                fprintf(stderr, "Failed to load the datastream\n");
                return -1;
        }
        datastream_list_timeseries(d, list_timeseries_callback, NULL);
        return 0;
}

static int list_datapoints(osdb_t* osdb, id_t id, timestamp_t start, timestamp_t end) 
{
        osdb_list_callbacks_t callbacks = { NULL, start_callback, data_callback, end_callback };
        osdb_datastream_list(osdb, id, start, end, &callbacks);
        return 0;
}

static int delete_datapoints(osdb_t* osdb, id_t id, timestamp_t start, timestamp_t end) 
{
        return osdb_datastream_delete(osdb, id, start, end);
}

int main(int argc, char** argv)
{
        int arg1 = 1;

        while (1) {
                int option_index = 0;

                static struct option long_options[] = {
                        {"help", 0, 0, 'h'},
                        {"version", 0, 0, 'v'},
                        {"dir", 1, 0, 'd'},
                        {0, 0, 0, 0}
                };

                int c = getopt_long(argc, argv, "d:hv", 
                                long_options, &option_index);
                if (c == -1) {
                        break;
                }

                switch (c) {
                case 0:
                        printf ("option %s", long_options[option_index].name);
                        if (optarg) {
                                printf (" with arg %s", optarg);
                        }
                        printf ("\n");
                        break;
                case 'd':
                        _dir = optarg;
                        break;
                case 'v':
                        print_version(); /* and don't come back */
                        break;
                case 'h':
                        print_help();
                        break;
                case '?':
                        printf ("Unknown option %c\n", optopt);
                        print_usage();
                        exit(0);
                        break;
                default:
                        printf ("?? getopt returned character code 0%o ??\n", c);
                }
        }

        arg1 = optind;

        if (arg1 >= argc)
                print_usage();

        //---------------------------------------

        osdb_t* osdb = osdb_init();
        osdb_set_option(osdb, "dir", _dir);

        //---------------------------------------

        if (strcmp(argv[arg1], "import") == 0) {
                if (arg1 + 1 < argc) {
                        import_csv(argv[arg1 + 1]);
                }

        } else if (strcmp(argv[arg1], "timeseries") == 0) {
                if (arg1 + 1 < argc) {
                        uint32_t id = atol(argv[arg1 + 1]);
                        list_timeseries(id);
                }

        } else if (strcmp(argv[arg1], "select") == 0) {
                if (arg1 + 3 < argc) {
                        uint32_t id = atol(argv[arg1 + 1]);
                        timestamp_t start = atof(argv[arg1 + 2]);
                        timestamp_t end = atof(argv[arg1 + 3]);
                        list_datapoints(osdb, id, start, end);
                }

        } else if (strcmp(argv[arg1], "delete") == 0) {
                if (arg1 + 3 < argc) {
                        uint32_t id = atol(argv[arg1 + 1]);
                        timestamp_t start = atof(argv[arg1 + 2]);
                        timestamp_t end = atof(argv[arg1 + 3]);
                        delete_datapoints(osdb, id, start, end);
                }
        }

        osdb_finalize(osdb);

        return 0;
}
