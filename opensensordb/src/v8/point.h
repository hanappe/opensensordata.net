#ifndef POINTV8
#define POINTV8

#include <v8.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

using namespace v8;

struct Point {

        Point() : x(0), y(0) { }              
        
        Point(int px, int py) : x(px), y(py) { }
        
        int x, y;
        
        struct JS {
                
                static void Initialize(Handle<ObjectTemplate> global, Isolate* i) {
                        global->Set(String::NewFromUtf8(i, "Point"), FunctionTemplate::New(i, Constructor));
                }

                static void Constructor(const FunctionCallbackInfo<Value>& info ) {
                        v8::Isolate* i = v8::Isolate::GetCurrent();
                        
                        Handle<ObjectTemplate> pt = ObjectTemplate::New();
                        pt->SetInternalFieldCount(1);
                        
                        pt->SetAccessor(String::NewFromUtf8(i, "x"), GetX, SetX);
                        pt->SetAccessor(String::NewFromUtf8(i, "y"), GetY, SetY);
                        
                        Point* point = 0;
                        
                        const Handle<Value>& arg1 = info[0];
                        const Handle<Value>& arg2 = info[1];
                        //Handle<Value> arg2 = info[1];
                        
                        if(!arg1.IsEmpty() && arg1->IsNumber() &&
                           !arg2.IsEmpty() && arg2->IsNumber()) {
                                point = new Point(arg1->Int32Value(), arg2->Int32Value());
                        } else {
                                point = new Point(0, 0);
                                
                        }
                        
			//Set Methods
			pt->Set(String::NewFromUtf8(i, "reset"), FunctionTemplate::New(i, Reset));
			pt->Set(String::NewFromUtf8(i, "version"), FunctionTemplate::New(i, Version));
			pt->Set(String::NewFromUtf8(i, "array"), FunctionTemplate::New(i, Array));
                        
                        Local<Object> obj = pt->NewInstance();
                        obj->SetInternalField(0, External::New(i, point));
                        
                        // Create and Return this newly created object
                        info.GetReturnValue().Set(obj);
                        
                }
                static void GetX(Local<String> property,
                                 const PropertyCallbackInfo<Value> &info) {
                        Local<Object> self = info.Holder();
                        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
                        void* ptr = wrap->Value();
                        int value = static_cast<Point*>(ptr)->x;
                        
                        info.GetReturnValue().Set(Integer::New(info.GetIsolate(), value));
                }
                
                static void SetX(Local<String> property, Local<Value> value,
                                 const PropertyCallbackInfo<void>& info) {
                        Local<Object> self = info.Holder();
                        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
                        void* ptr = wrap->Value();
                        static_cast<Point*>(ptr)->x = value->Int32Value();
                }
                
                static void GetY(Local<String> property,
                                 const PropertyCallbackInfo<Value> &info) {
                        Local<Object> self = info.Holder();
                        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
                        void* ptr = wrap->Value();
                        int value = static_cast<Point*>(ptr)->y;
                        
                        info.GetReturnValue().Set(Integer::New(info.GetIsolate(), value));
                }
                
                static void SetY(Local<String> property, Local<Value> value,
                                 const PropertyCallbackInfo<void>& info) {
                        Local<Object> self = info.Holder();
                        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
                        void* ptr = wrap->Value();
                        static_cast<Point*>(ptr)->y = value->Int32Value();
                }
                
                // Set point position (x,y) to 0; 
                static void Reset(const FunctionCallbackInfo<Value>& info){
                        
                        Local<Object> self = info.Holder();
                        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
                        void* ptr = wrap->Value();

                        Point * p = static_cast<Point*>(ptr);
                        p->x = 0;
                        p->y = 0;
                }
	  
                static void Version(const v8::FunctionCallbackInfo<v8::Value>& args) {
                        args.GetReturnValue().Set(
                                                  v8::String::NewFromUtf8(args.GetIsolate(), v8::V8::GetVersion()));
                }
                
                static std::string ObjectToString(Local<Value> value) {
                        String::Utf8Value utf8_value(value);
                        return std::string(*utf8_value);
                }
                
                static void Array(const v8::FunctionCallbackInfo<v8::Value>& args) {
                        
                        
                        /*v8::Isolate* isolate = v8::Isolate::GetCurrent();       
                        // We will be creating temporary handles so we use a handle scope.
                        HandleScope handle_scope(isolate);
                        // Create a new empty array.
                        Handle<Array> array = Array::New(isolate, 3);
                        // Return an empty result if there was an error creating the array.
                        if (array.IsEmpty())
                        return Handle<Array>();
                        // Fill out the values
                        array->Set(0, Integer::New(isolate, x));
                        array->Set(1, Integer::New(isolate, y));                                                                                                        
                        array->Set(2, Integer::New(isolate, z));
                        // Return the value through Close.
                        return handle_scope.Close(array);
                        */
                        
                        /*	    ocal<Object> global = Context::GetCurrent()->Global();
                                    
                        // Get JSON
                        // Same as using 'global.JSON'
                        Local<Object> JSON = Local<Object>::Cast(
                        global->Get(String::New("JSON")));
                        
                        // Get stringify
                        // Same as using 'global.JSON.stringify'
                        Local<Function> stringify = Local<Function>::Cast(
                        JSON->Get(String::New("stringify")));
                        
                        // Stringify the object
                        // Same as using 'global.JSON.stringify.apply(global.JSON, [ obj ])
                        Local<Value> args[] = { obj };
                        Local<String> result = Local<String>::Cast(stringify->Call(JSON, 1, args));
                        
                        args.GetReturnValue().Set(array);
                        */
                        
                        //BasicJsonStringifier stringifier(isolate);
                        
                        v8::Isolate* isolate = v8::Isolate::GetCurrent();       
                        //v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
                        
                        
                        
                        v8::Handle<v8::Array> array = v8::Array::New(isolate, 3);
                        
                        array->Set(0, Integer::New(isolate, 3));
                        array->Set(1, Integer::New(isolate, 2));                                                                                                        
                        array->Set(2, Integer::New(isolate, 1));
                        
                        
                        //LocalContext env;
                        //v8::Isolate* isolate = env->GetIsolate();
                        
                        //Handle<ObjectTemplate> global = ObjectTemplate::New(isolate);
                        //global->Set(String::NewFromUtf8(isolate, "obj"), v8::Array::New(isolate, 3));
                        
                        
                        //global->Set(v8::String::NewFromUtf8(GetIsolate(), "obj"), v8::Array::New(GetIsolate(), 3));
                        //v8::Handle<v8::String> expected = v8_str("{\"regress\":\"crbug-161028\"}");
                        //CHECK();
                        
                        
                        Handle<String> source = String::NewFromUtf8(isolate, "JSON.stringify(obj)");
                        
                        Handle<Script> script = Script::Compile(source);
                        
                        HandleScope handle_scope(isolate);
                        
                        Handle<ObjectTemplate> global1 = ObjectTemplate::New(isolate);
                        v8::Handle<v8::Context> context = Context::New(isolate, NULL, global1);
                        
                        
                        
                        
                        //args.GetContext();
                        
                        //v8::Local<v8::Context> context = Context::GetCurrent();//xv8::Local<v8::Context>::New(isolate);
                        //v8::Context::Scope context_scope(context);
                        //Handle<Object> global = context->Global();
                        
                        //global->Set(v8::String::NewFromUtf8(GetIsolate(), "obj"), v8::Array::New(GetIsolate(), 3));
                        //Handle<Value> fun = global1->Get(String::NewFromUtf8(isolate, "Process"));
                        /*if (fun->IsFunction()) {
                          printf("IS FUNCTION");
                          } else {
                          printf("Is not a function");
                          }*/
                        
                        std::string s = ObjectToString(array);
                        printf(s.data());
                        //static const int kArgc = 1;
                        //Handle<Value> argv[kArgc] = { array };
                        //Handle<Value> val = Handle<Function>::Cast(fun)->Call(global, kArgc, argv);
                        //return handle_scope.CloseAndEscape(Local<Value>(val));
                        
                        
                        
                        //v8::internal::BasicJsonStringifier stringifier(isolate);
                        //Handle<Object> result;
                        //stringifier.Stringify(object);
                        // Run the script to get the result.
                        //Handle<Value> result = ;
                        
                        // Convert the result to an UTF8 string and print it.
                        //String::Utf8Value utf8(result);
                        //printf("%s\n", *utf8);
                        
                        args.GetReturnValue().Set(array->ToDetailString());
                }
                
                
        }; //end class JS

}; // end class Point

#endif // POINTV8

