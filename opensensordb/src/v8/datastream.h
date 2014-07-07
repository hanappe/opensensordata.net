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

using namespace v8;


struct Datapoint {

        char strtime[256];
        time_t time;
        double value;

	void print() const;
};

class Datastream {
public:

	int id;
	int ownerId;
	char name[512];
	char description[512];
        
        // Classic Storage
	std::vector<Datapoint> datapoints;

        // "Contiguous" storage
	typedef std::vector<Datapoint> Datarow;
	std::vector<Datarow*> datarows;

	Datarow * current;

public:

        Datastream();
	~Datastream();

        bool load(int id);
        bool loadContiguous(int id);
	void print() const;
	void clear();
        void clearContiguous();

	Handle<Array> toV8Array(Isolate * i) const;
        Handle<Array> toV8ArrayContiguous(Isolate * i);

        // Simple way

        void appendDatapoints(char * strtime, time_t time, double value) {
                Datapoint d;
                strcpy(d.strtime, strtime);
                d.time = time;
                d.value = value;

                datapoints.push_back(d);
	}

        // Contiguous way
	
	void beginDatarow() {
                current = new Datarow;
	}

	void append(char * strtime, time_t time, double value) {
                if (!current) {
                        beginDatarow();
                }
                Datapoint d;
                strcpy(d.strtime, strtime);
                d.time = time;
                d.value = value;
                current->push_back(d);
	}

	void endDatarow() {
                if (current) {
                        datarows.push_back(current);
                        current = 0;
                }
	}

	
        struct JS {

                // Create a mew instance of javascript object "Datastream"
                static Local<Object> GetNewInstance(Isolate * i);

                // Register function wich create javascript object "Datastream" in global scope
                static void Register(Handle<ObjectTemplate> global, Isolate* i);                
                static void Constructor(const FunctionCallbackInfo<Value>& info);
                static void Load(const FunctionCallbackInfo<Value>& info);
                static void Select(const FunctionCallbackInfo<Value>& info);
                static void ComputationTime(Local<String> property, const PropertyCallbackInfo<Value> &info);

                static void GetDatapoints(Local<String> property, const PropertyCallbackInfo<Value> &info);
                static void GetX(Local<String> property, const PropertyCallbackInfo<Value> &info);

                // Fill 'obj' with datastream properties
                static void SetupObject(Local<Object> obj, const Datastream * d, Isolate* i);
                
        }; // end struc JS
};

#endif // DATASTREAMV8_H

