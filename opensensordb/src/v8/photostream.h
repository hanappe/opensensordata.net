#ifndef PHOTOSTREAMV8_H
#define PHOTOSTREAMV8_H

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


struct PhotoInfo {

        int photostreamId;
        //int year;
        //int month;
        //int day;
        //int hour;
        //int minute;
        //int second;
        char strdatetime[512];
        time_t datetime;
        //timezone;
        //longitude;
        //latitude;
        char filename[512];
        //upload_date;

	void print() const;
};


class Photostream {
public:

	int id;
	int ownerId;
	char name[512];
	char description[512];
        
	std::vector<PhotoInfo> photoinfos;

public:
        Photostream();
	
	~Photostream();

        bool load(int id);
	void print() const;

	void clear();

        void append(const PhotoInfo& photo);
	
        struct JS {

                // Create a mew instance of javascript object "Photostream"
                static Local<Object> GetNewInstance(Isolate * i);

                // Register function wich create javascript object "Photostream" in global scope
                static void Register(Handle<ObjectTemplate> global, Isolate* i);                
                static void Constructor(const FunctionCallbackInfo<Value>& info);
                static void Load(const FunctionCallbackInfo<Value>& info);
                static void Select(const FunctionCallbackInfo<Value>& info);
                static void GetDatapoints(Local<String> property, const PropertyCallbackInfo<Value> &info);
                static void GetX(Local<String> property, const PropertyCallbackInfo<Value> &info);

                // Fill 'obj' with photostream properties
                static void SetupObject(Local<Object> obj, const Photostream * p, Isolate* i);
                
        }; // end struc JS

}; // end class Photostrean

#endif // PHOTOSTREAMV8_H

