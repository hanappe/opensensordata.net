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
        bool loadFromRange(int id, time_t start, time_t end);
	void print() const;
	void clear();

        Handle<Array> select(Isolate * i, time_t start, time_t end) const;
        Handle<Array> select2(Isolate * i, time_t start, time_t end) const;

	Handle<Array> toV8Array(Isolate * i) const;
        Handle<Array> toV8ArrayContiguous(Isolate * i);

        // Simple way

        void appendDatapoints(char * strtime, time_t time, double value);

        // Contiguous way
	
	void beginDatarow();
	void append(char * strtime, time_t time, double value);
	void endDatarow();
	
        struct JS {

                static Handle<ObjectTemplate> GetNewTemplate(Isolate * i);

                // Register function wich create javascript object "Datastream" in global scope
                static void Register(Handle<ObjectTemplate> global, Isolate* i);                
                static void Constructor(const FunctionCallbackInfo<Value>& info);
                static void Load(const FunctionCallbackInfo<Value>& info);
                static void Select(const FunctionCallbackInfo<Value>& info);
                static void Select2(const FunctionCallbackInfo<Value>& info);
                static void SelectZ(const FunctionCallbackInfo<Value>& info);
                static void ComputationTime(const FunctionCallbackInfo<Value>& info);
                static void TotalTime(const FunctionCallbackInfo<Value>& info);

                static void GetDatapoints(Local<String> property, const PropertyCallbackInfo<Value> &info);
                static void GetX(Local<String> property, const PropertyCallbackInfo<Value> &info);

                // Fill 'obj' with datastream properties
                static void SetupObject(Local<Object> obj, const Datastream * d, Isolate* i);

                // Reference to all cpp object (needed to delete them)
                static std::vector<Datastream*> references;

                static void AddToRef(Datastream * datastream);
                static void DeleteAllRef();
                
        }; // end struct JS
};

#endif // DATASTREAMV8_H

