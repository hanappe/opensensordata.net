#ifndef DATASTREAMV8_H
#define DATASTREAMV8_H

#include <v8.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include <sstream>
#include <iostream>
#include <vector>

#include "utils.h"
//#include "mdb.h"

using namespace v8;


struct Pair {
  Pair() {
  }
  
  Pair(time_t t, double v) : time(t), value(v) {
  
  }

  Pair(const Pair& p) : time(p.time), value(p.value) {
  
  }

  time_t time;
  double value;
};


class Datastream {
public:

        Datastream(int id = 0) : mId(id), current(0x0) {

	}

	int mId;
	
	~Datastream() {
	  clear();
	}

	void clear() {
	  for (std::vector<Datarow*>::iterator it = datarows.begin(); it != datarows.end(); ++it) {
	    delete (*it);
	  };
	  datarows.clear();
	  current = 0x0;
	}
	
	void beginDatarow() {
	  current = new Datarow;
	}

	void append(int date, double value) {
	  
	  if (!current) {
	    beginDatarow();
	  }
	  Pair p(date, value);
	  //std::cout << "Pair date: " << p.date << " v: " << p.value << current << std::endl;
	  current->push_back(p);
	}

	void endDatarow() {
	  if (current) {
	    datarows.push_back(current);
	    current = 0;
	  }
	}

	void print() {
	  /*
	  std::cout << "Datastreqm print begin" << std::endl;
	  std::cout << "data row count: " << datarows.size() << std::endl;
	  for (std::vector<Datarow*>::const_iterator it = datarows.begin(); it != datarows.end(); ++it) {

	    const Datarow * dr = (*it);
	    std::cout << "datapoints count: " << dr->size() << std::endl;
	    for (std::vector<Pair>::const_iterator itb = dr->begin(); itb != dr->end(); ++itb) {
	      const Pair& p = (*itb);
	      //std::cout << "date: " << p.date << " value: " << p.value << std::endl;
	    };
	  };
	  */
	};

	Handle<Array> toV8Array(Isolate * i);

	typedef std::vector<Pair> Datarow;
	std::vector<Datarow*> datarows;

	Datarow * current;
	
        struct JS {
                
	  static void Initialize(Handle<ObjectTemplate> global, Isolate* i) {
	    fprintf(stdout, "Datastream Initialize\n");
	    global->Set(String::NewFromUtf8(i, "Datastream"), FunctionTemplate::New(i, Constructor));
	  }

	  static void Constructor(const FunctionCallbackInfo<Value>& info);
	  static void GetDatapoints(Local<String> property, const PropertyCallbackInfo<Value> &info);
	  static void GetX(Local<String> property, const PropertyCallbackInfo<Value> &info);
	  static void Select(const FunctionCallbackInfo<Value>& info);


        }; // end struc JS
};

#endif // DATASTREAMV8_H

