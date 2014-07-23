
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

        // TODO remove this unused function 
        inline void InitializeV8() {
                v8::V8::InitializeICU();
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
                global->Set(v8::String::NewFromUtf8(isolate, "exit"),
                            v8::FunctionTemplate::New(isolate, Exit));
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

                //printf("args<\n");
                while (args) {
                        pair_t* p = (pair_t*) args->data;

                        //printf("%s = %s\n", p->name, p->value);
                        //std::cout << "test:[" << p->value << "]" <<  std::endl;
                        if (p->value) {
                                //std::cout << "test2:[" << p->value << "]" <<  std::endl;
                                args_obj->Set(v8::String::NewFromUtf8(isolate, p->name),
                                              v8::String::NewFromUtf8(isolate, p->value));
                        }

                        args = args->next;
                }
                //printf(">\n");
                
                return args_obj;
        }


        void InitiliazeGlobalArgs(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global, list_t* args) {
                global->Set(String::NewFromUtf8(isolate, "args"), createArgs(isolate, args));
        }

        // TODO remove this unused function
        inline void ReleaseV8() {

                // ~Isolate() is private. :fu:

                //if (isolate) {
                //        delete isolate;
                //}

                v8::V8::Dispose();
        }

enum ScriptError {
        UNIMPL,
        UNDEF,
        EMPTY,
        CONTEXT,
        COMPILESCRIPT,
        RUNSCRIPT,
        EMPTYRES
};

char * GetError(ScriptError e, char * msg) {
        char buf[512];
        char error[256];

        switch(e) {
        case UNIMPL : { sprintf(error, "Not yet implemented"); break;}
        case UNDEF : { sprintf(error, "Undefined error"); break;}
        case EMPTY : { sprintf(error, "Empty source"); break;}
        case CONTEXT : { sprintf(error, "JSEngine: error creating context"); break;}
        case COMPILESCRIPT : { sprintf(error, "JSEngine: error compiling script"); break;}
        case RUNSCRIPT : { sprintf(error, "JSEngine: error running script"); break;}
        case EMPTYRES : { sprintf(error, "JSEngine: error empty result"); break;}
        }
        if (msg) {
                snprintf(buf, sizeof buf, "{ \"errno\": %d, \"message\": \"%s : %s\" }", e, error, msg);
        } else {
                snprintf(buf, sizeof buf, "{ \"errno\": %d, \"message\": \"%s\" }", e, error);
        }

        return strdup(buf);
}

char * GetError(ScriptError e) {
        return GetError(e, NULL);
}

//}

/*
#define ERROR_UNIMPL "{ \"errno\": 1, \"message\": \"Not yet implemented\" }"
#define ERROR_UNDEF "{ \"errno\": 2, \"message\": \"Undefined Error\" }"
#define ERROR_EMPTY "{ \"errno\": 3, \"message\": \"Empty source\" }"
#define ERROR_CONTEXT "{ \"errno\": 4, \"message\": \"JSEngine: Error creating context\" }"
#define ERROR_COMPILESCRIPT "{ \"errno\": 5, \"message\": \"JSEngine: Error compiling script\" }"
#define ERROR_RUNSCRIPT "{ \"errno\": 6, \"message\": \"JSEngine: Error running script\" }"
*/

char* server_execute_script(const char* code, list_t* args) {

                
        if (!code) {
                fprintf(stderr, "Empty source code\n");
                return GetError(ScriptError::EMPTY);
        }
        
        {
                Isolate* isolate = Isolate::New();
                v8::Isolate::Scope isolate_scope(isolate);
                v8::HandleScope handle_scope(isolate);
                
                // Create a template for the global object.
                v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
                
                InitializeCustom(isolate, global);

                InitiliazeGlobalArgs(isolate, global, args);

                v8::Handle<v8::Context> context = v8::Context::New(isolate, NULL, global);

                if (!context.IsEmpty()) {

                        v8::Context::Scope context_scope(context);

                        v8::TryCatch try_catch;
                        Handle<String> source = String::NewFromUtf8(isolate, code);
                        Handle<Script> script = Script::Compile(source);

                        if (try_catch.HasCaught()) {
                                v8::String::Utf8Value exception(try_catch.Exception());
                                return GetError(ScriptError::COMPILESCRIPT, *exception);
                        }

                        Handle<Value> result = script->Run();

                        if (try_catch.HasCaught()) {
                                v8::String::Utf8Value exception(try_catch.Exception());
                                if (*exception) {
                                        // Script ended with "exit()" function
                                        if (strcmp(*exception, "Exit") == 0) {
                                                //std::cout << "In Exit:" << *exception << std::endl;
                                                String::Utf8Value utf8res(result);
                                                //std::cout << "In Exit: " << (*utf8res != NULL) << std::endl;
                                                // Temp debug
                                                if (*utf8res) {
                                                        printf("RUN RESULT exit: %s\n", *utf8res);
                                                }

                                                return (*utf8res != NULL) ? strdup(*utf8res) : GetError(ScriptError::EMPTYRES);

                                        } else {
                                                return GetError(ScriptError::RUNSCRIPT, *exception);
                                        }

                                }
                                //v8::Handle<v8::Message> message = try_catch->Message();
                                
                                return GetError(ScriptError::RUNSCRIPT);
                        }                        
                        printf("</run>\n");

                        
                        String::Utf8Value utf8res(result);

                        // Temp debug
                        if (*utf8res) {
                                printf("RUN RESULT A: %s\n", *utf8res);
                        }

                        return (*utf8res != NULL) ? strdup(*utf8res): GetError(ScriptError::EMPTYRES);
                } else {
                        return GetError(ScriptError::CONTEXT);
                }

                //context.Dispose();
                
                //V8::Dispose();
        }

        return GetError(ScriptError::UNIMPL);
}

void server_free_result(char* buffer)
{
        if (buffer) {
                free(buffer);
        }
}
