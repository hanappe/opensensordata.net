
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "server.h"
#include "osdb.h"

#include "v8/jsengine.h"
#include "v8/point.h"
#include "v8/group.h"
#include "v8/datastream.h"
#include "v8/photostream.h"


using namespace v8;

//namespace {
        

        v8::Isolate* isolate;

        v8::Isolate* GetIsolate() {
                return isolate;
        }

        void InitializeV8() {
                v8::V8::InitializeICU();
                isolate = v8::Isolate::New();                
        }
        
        void InitializeCustom(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global) {
                // Bind the global 'print' function to the C++ Print callback.        
                global->Set(v8::String::NewFromUtf8(isolate, "print"),
                            v8::FunctionTemplate::New(isolate, Print));

                // Bind the global 'read' function to the C++ Read callback.
                global->Set(v8::String::NewFromUtf8(isolate, "read"),
                            v8::FunctionTemplate::New(isolate, Read));
                // Bind the global 'load' function to the C++ Load callback.
                global->Set(v8::String::NewFromUtf8(isolate, "load"),
		v8::FunctionTemplate::New(isolate, Load));
                // Bind the 'quit' function
                global->Set(v8::String::NewFromUtf8(isolate, "quit"),
                            v8::FunctionTemplate::New(isolate, Quit));
                // Bind the 'version' function
                global->Set(v8::String::NewFromUtf8(isolate, "version"),
                            v8::FunctionTemplate::New(isolate, Version));

                Point::JS::Register(global, isolate);                
                Group::JS::Register(global, isolate);
                Datastream::JS::Register(global, isolate);
                Photostream::JS::Register(global, isolate);

        
        }

        // Create the global javascript variable "args" and fill it with args list
        Handle<ObjectTemplate> createArgs(Isolate* isolate, list_t* args) {

                
                Handle<ObjectTemplate> args_obj = ObjectTemplate::New(isolate);

                printf("args<\n");
                while (args) {
                        pair_t* p = (pair_t*) args->data;

                        printf("%s = %s\n", p->name, p->value);
                        //std::cout << "test:[" << p->value << "]" <<  std::endl;
                        if (p->value) {
                                //std::cout << "test2:[" << p->value << "]" <<  std::endl;
                                args_obj->Set(v8::String::NewFromUtf8(isolate, p->name),
                                              v8::String::NewFromUtf8(isolate, p->value));
                        }

                        args = args->next;
                }
                printf(">\n");
                
                return args_obj;
        }


        void InitiliazeGlobalArgs(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global, list_t* args) {
                global->Set(String::NewFromUtf8(isolate, "args"), createArgs(isolate, args));
        }

        void ReleaseV8() {
                v8::V8::Dispose();
        }

//}


#define ERROR_UNIMPL "{ \"errno\": 1, \"message\": \"Not yet implemented\" }"
#define ERROR_UNDEF "{ \"errno\": 2, \"message\": \"Undefined Error\" }"
#define ERROR_EMPTY "{ \"errno\": 3, \"message\": \"Empty source\" }"
#define ERROR_CONTEXT "{ \"errno\": 4, \"message\": \"JSEngine: Error creating context\" }"


char* server_execute_script(const char* code, list_t* args) {
        
        if (!code) {
                fprintf(stderr, "Empty source code\n");
                return strdup(ERROR_EMPTY);
        }
        
        InitializeV8();
        
        v8::Isolate* isolate = GetIsolate();
        
        {
                v8::Isolate::Scope isolate_scope(isolate);
                v8::HandleScope handle_scope(isolate);
                
                // Create a template for the global object.
                v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
                
                InitializeCustom(isolate, global);

                InitiliazeGlobalArgs(isolate, global, args);

                v8::Handle<v8::Context> context = v8::Context::New(isolate, NULL, global);

                if (!context.IsEmpty()) {

                        v8::Context::Scope context_scope(context);

                        Handle<String> source = String::NewFromUtf8(isolate, code);
                        Handle<Script> script = Script::Compile(source);
                        Handle<Value> result = script->Run();
                        String::Utf8Value utf8b(result);
                        
                        //printf("CODE: %s\n", code);
                        printf("RUN RESULT: %s\n", *utf8b);
                        return strdup(*utf8b);

                } else {
                        return strdup(ERROR_CONTEXT);
                }
                
        }

        ReleaseV8();
               
        return strdup(ERROR_UNIMPL);
}

void server_free_result(char* buffer)
{
        free(buffer);
}
