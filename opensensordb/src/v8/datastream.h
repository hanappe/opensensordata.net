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
        //Pair() {}
        
        //Pair(time_t t, double v) : time(t), value(v) {}

        //Pair(const Pair& p) : time(p.time), value(p.value) {}

        char strtime[256];
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


        bool load(int id);
        bool loadContiguous(int id);
	void print();

	Handle<Array> toV8Array(Isolate * i);
        Handle<Array> toV8ArrayContiguous(Isolate * i);

	void clear() {
	  for (std::vector<Datarow*>::iterator it = datarows.begin(); it != datarows.end(); ++it) {
	    delete (*it);
	  };
	  datarows.clear();
	  current = 0x0;
          //TODO free all strtime
	}

        // Simple way

        void appendDatapoints(char * strtime, time_t time, double value) {
                Pair p;
                strcpy(p.strtime, strtime);
                p.time = time;
                p.value = value;

                datapoints.push_back(p);
                
	}

        // Contiguous way
	
	void beginDatarow() {
	  current = new Datarow;
	}

	void append(char * strtime, time_t time, double value) {
	  
	  if (!current) {
	    beginDatarow();
	  }
	  Pair p;
          strcpy(p.strtime, strtime);
          p.time = time;
          p.value = value;
	  //std::cout << "Pair date: " << p.date << " v: " << p.value << current << std::endl;
	  current->push_back(p);
	}

	void endDatarow() {
	  if (current) {
	    datarows.push_back(current);
	    current = 0;
	  }
	}


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


        // Classic Storage
	std::vector<Pair> datapoints;

        // "Contiguous" storage
	typedef std::vector<Pair> Datarow;
	std::vector<Datarow*> datarows;

	Datarow * current;
	
        struct JS {
                
                static void Initialize(Handle<ObjectTemplate> global, Isolate* i);
                
                static void Constructor(const FunctionCallbackInfo<Value>& info);
                static void Load(const FunctionCallbackInfo<Value>& info);
                static void Select(const FunctionCallbackInfo<Value>& info);
                static void GetDatapoints(Local<String> property, const PropertyCallbackInfo<Value> &info);
                static void GetX(Local<String> property, const PropertyCallbackInfo<Value> &info);

        }; // end struc JS
};

#endif // DATASTREAMV8_H

