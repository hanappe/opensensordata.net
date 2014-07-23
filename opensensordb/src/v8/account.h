#ifndef ACCOUNTV8_H
#define ACCOUNTV8_H

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

#include "group.h"


class Group;

using namespace v8;

class Account {
public:

        Account() : id(0) {
	}
	
	~Account() {
                clear();
	}

        bool loadFromId(int id);
        bool loadFromUsername(int id);
	void print();
	void clear();

        Handle<Array> select(Isolate * i, time_t start, time_t end) const;

	int id; // Account Id
        char username[512];
        char email[512];

	std::vector<Group*> groups;
	
        struct JS {

                // Create a mew instance of javascript object "Group"
                static Local<Object> GetNewInstance(Isolate * i);

                // Register function wich create javascript object "Group" in global scope
                static void Register(Handle<ObjectTemplate> global, Isolate* i);
                static void Constructor(const FunctionCallbackInfo<Value>& info);
                static void Load(const FunctionCallbackInfo<Value>& info);
                static void Select(const FunctionCallbackInfo<Value>& info);

                // Fill 'obj' with group properties
                static void SetupObject(Local<Object> obj, Account * a, Isolate* i);
        }; // end struct JS
};

#endif // ACCOUNTV8_H

