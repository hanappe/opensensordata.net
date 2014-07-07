#ifndef GROUPV8_H
#define GROUPV8_H

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

#include "datastream.h"


class Datastream;
class Photostream;

using namespace v8;

class Group {
public:

        Group(int i = 0) : id(i) {
	}
	
	~Group() {
                clear();
	}

        bool load(int id);
	void print();
	void clear();

	int id; // Group Id
	int ownerId; // Owner Id
        char name[256];

	std::vector<Datastream*> datastreams;
	std::vector<Photostream*> photostreams;
	
        struct JS {

                // Create a mew instance of javascript object "Datastream"
                static Local<Object> GetNewInstance(Isolate * i);

                // Register function wich create javascript object "Datastream" in global scope
                static void Register(Handle<ObjectTemplate> global, Isolate* i);
                static void Constructor(const FunctionCallbackInfo<Value>& info);
                static void Load(const FunctionCallbackInfo<Value>& info);

                // Fill 'obj' with datastream properties
                static void SetupObject(Local<Object> obj, Group * g, Isolate* i);
        }; // end struc JS
};

#endif // GROUPV8_H

