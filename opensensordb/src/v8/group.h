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

        Group(int i = 0);
	~Group();

        bool load(int id);
	void print();
	void clear();

        Handle<Array> select(Isolate * i, time_t start, time_t end) const;

	int id; // Group Id
	int ownerId; // Owner Id
        char name[512];

	std::vector<Datastream*> datastreams;
	std::vector<Photostream*> photostreams;
	
        struct JS {

                // Create a mew instance of javascript object "Datastream"
                static Handle<ObjectTemplate> GetNewTemplate(Isolate * i);

                // Register function wich create javascript object "Datastream" in global scope
                static void Register(Handle<ObjectTemplate> global, Isolate* i);
                static void Constructor(const FunctionCallbackInfo<Value>& info);
                static void Load(const FunctionCallbackInfo<Value>& info);
                static void Select(const FunctionCallbackInfo<Value>& info);

                // Fill 'obj' with datastream properties
                static void SetupObject(Local<Object> obj, Group * g, Isolate* i);
                
                // Reference to all cpp object (needed to delete them)
                static std::vector<Group*> references;

                static void AddToRef(Group * group);
                static void DeleteAllRef();
                
        }; // end struct JS
};

#endif // GROUPV8_H

