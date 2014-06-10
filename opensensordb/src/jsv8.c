#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "osdb.h"

#include "v8/jsengine.h"

/*
namespace {
        

        v8::Isolate* isolate;

        v8::Isolate* GetIsolate() {
                return isolate;
        }

        void InitializeV8() {
                v8::V8::InitializeICU();
                isolate = v8::Isolate::New();                
        }
        
        void InitializeCustom(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global) {
                std::cout << "Initialize global variable" << std::endl;

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

                Point::JS::Initialize(global, isolate);
                
                Datastream::JS::Initialize(global, isolate);
        
        }

        // Create the global javascript variable "args"
        Handle<ObjectTemplate> createArgs(Isolate* isolate, const char * key, const char * value) {
                //HandleScope handle_scope(isolate);
                
                Handle<ObjectTemplate> args = ObjectTemplate::New(isolate);
                //Gl->SetInternalFieldCount(1);
                args->Set(v8::String::NewFromUtf8(isolate, "quit"),
                          v8::String::NewFromUtf8(isolate, "test"));
                
                //const char * key = "key";
                //const char * val = "value";
                
                args->Set(v8::String::NewFromUtf8(isolate, key),
                          v8::String::NewFromUtf8(isolate, value));
                
                return args;
        }


       void InitiliazeGlobalArgs(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global) {
 
               
               //global->Set(String::NewFromUtf8(isolate, "args"), createArgs(isolate));

        }

        void ReleaseV8() {
                v8::V8::Dispose();
        }
}
*/

char* server_execute_script(const char* code, list_t* args)
{

        //InitializeV8();
        printf("code:\n");
        printf(code);
        printf("args:\n");
        list_t * l = args;
        while (l) {
                pair_t* p = (pair_t*) l->data;
                printf("args[]: %s = %s\n", p->name, p->value);
                l = l->next;
        }

        return strdup("{ \"errno\": 1, \"message\": \"Not yet implemented\" }");
}

void server_free_result(char* buffer)
{
        free(buffer);
}
