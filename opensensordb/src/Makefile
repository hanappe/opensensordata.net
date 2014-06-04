

all: osd_test osdb_server

osd_test: main.c osdb.c osdb.h osdb_priv.h datapoint.c datapoint.h filemanager.h filemanager.c value.h value.c timestamp.h timestamp.c fileio.h fileio.c timeseries.h timeseries.c datastream.h datastream.c list.h list.c hashtable.h hashtable.c log.h log.c
	gcc -g -O0 -Wall -std=c99 main.c osdb.c datapoint.c filemanager.c value.c timestamp.c fileio.c timeseries.c datastream.c list.c hashtable.c log.c -lm -o osd_test

osdb_server: server.h server.c server_handler.c log.h log.c list.h list.c
	gcc -g -O0 -Wall -std=c99 server.c server_handler.c log.c list.c -o osdb_server

