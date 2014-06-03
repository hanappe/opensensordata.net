# wu analyzer application 

SHELL = /bin/bash

CC = gcc
CFLAGS = -g -Wall -std=c99

CXX = g++
CXXFLAGS = -g -Wall -std=c++0x

V8PATH = ../v8
V8OUT = native

CXXFLAGS += -I./ \
	-I./$(V8PATH)/include/

#LDFLAGS += -L./depends/v8/lib/ \
	-L/usr/lib/mysql

#depends_src = $(addprefix ./ , datastream.cpp)
depends_src = datastream.cpp \
	db.cpp \
	point.cpp


application_srcs = jswrap.cpp $(depends_src)
application_deps = ${application_srcs:%.cpp=%.d}
application_objs = ${application_srcs:%.cpp=%.o}

#application_static_libs = $(addprefix ./depends/v8/lib/, libv8_base.x64.a \
	libv8_snapshot.a \
	libicuuc.a \
	libicui18n.a \
	libicudata.a)

#application_static_libs = libv8_base.x64.a libv8_snapshot.a libicuuc.a libicuil8n.a libicudata.a
#application_static_libs = libmysqlclient.a
#application_ldflags = -lv8
application_ldflags = -lmysqlclient
depends += $(application_deps)
objects += $(application_objs) $(application_static_libs)
programs += jswrap

generated = $(programs) $(objects) $(depends) stderr.txt

all: $(depends) $(programs)

#application: $(application_objs)
#	$(CXX) -I$(INCLUDEPATH) application.cpp -o testv8 $(LDFLAGS) $(application_static_libs)

jswrap: $(application_objs)
	$(CXX) $(CXXFLAGS) $(application_objs) -o jswrap -Wl,--start-group $(V8PATH)/out/$(V8OUT)/obj.target/{tools/gyp/libv8_{base.x64,snapshot},third_party/icu/libicu{uc,i18n,data}}.a -Wl,--end-group -lrt $(application_static_libs) -lmysqlclient


#%.d: %.c
#	$(CC) -o $@ $(CFLAGS) $< -MM -MT '$@ $*.o'

%.d: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) $< -MM -MT '$@ $*.o'

clean:
	rm -f $(generated)

ifneq ($(MAKECMDGOALS),clean)
-include $(depends)
endif
