SHELL = /bin/bash


jsengine_sources = v8/db.cpp v8/datastream.cpp v8/point.cpp v8/jsengine.cpp
jsengine_objects = ${jsengine_sources:.cpp=.o}
jsengine_libs = libosdb.a -lm


lib_sources = osdb.c datapoint.c filemanager.c value.c timestamp.c fileio.c timeseries.c datastream.c list.c hashtable.c log.c
lib_objects = ${lib_sources:.c=.o}
lib_libs = -lm

server_sources = server.c server_handler.c jsv8.c
server_objects = ${server_sources:.c=.o}
server_libs = -lm

cli_sources = main.c
cli_objects = ${cli_sources:.c=.o}
cli_libs = -lm


CFLAGS = -g -O0 -Wall -std=c99

CXXFLAGS = -g -Wall -std=c++0x
#CXXFLAGS += -I./


V8PATH = /home/low-energy-boinc/v8
V8OUT = native

CXXFLAGS += -I$(V8PATH)/include/ 

all: opensensordb osdb_cli 

#libosdb.a: $(lib_objects)
#	ar rvs $@ $^

osdb_cli: $(lib_objects) $(cli_objects)
	g++ -g $(CXXFLAGS) $(cli_objects) $(lib_objects) $(cli_libs) -o $@

opensensordb: $(lib_objects) $(server_objects) $(jsengine_objects)
	g++ -g $(CXXFLAGS) $(lib_objects) $(server_objects) $(jsengine_objects) $(server_libs) -o $@ -Wl,--start-group $(V8PATH)/out/$(V8OUT)/obj.target/{tools/gyp/libv8_{base.x64,snapshot},third_party/icu/libicu{uc,i18n,data}}.a -Wl,--end-group -lrt $(application_static_libs) -lmysqlclient

%.o: %.c
	g++ -c $(CXXFLAGS) $^ -o $@ 

%.o: %.cpp
	g++ -c $(CXXFLAGS) $^ -o $@ 

install: opensensordb
	cp opensensordb /usr/local/bin

clean:
	rm -f $(lib_objects) $(server_objects) $(cli_objects) $(jsengine_objects)

