lib_sources = osdb.c datapoint.c filemanager.c value.c timestamp.c fileio.c timeseries.c datastream.c list.c hashtable.c log.c
lib_objects = ${lib_sources:.c=.o}
lib_libs = -lm

server_sources = server.c server_handler.c jsv8.c
server_objects = ${server_sources:.c=.o}
server_libs = libosdb.a -lm

cli_sources = main.c
cli_objects = ${cli_sources:.c=.o}
cli_libs = libosdb.a -lm

CFLAGS = -g -O0 -Wall -std=c99

all: libosdb.a osdb_server osdb_cli

libosdb.a: $(lib_objects)
	ar rcs $@ $^

osdb_cli: libosdb.a $(cli_objects)
	gcc -g $(CFLAGS) $(cli_objects) $(cli_libs) -o $@

osdb_server: libosdb.a $(server_objects)
	gcc -g $(CFLAGS) $(server_objects) $(server_libs) -o $@

%.o: %.c
	gcc -c $(CFLAGS) $^ -o $@ 

clean:
	rm -f $(lib_objects) 
