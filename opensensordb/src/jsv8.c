
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "server.h"
#include "osdb.h"

#include "v8/jsengine.h"
#include "v8/point.h"
#include "v8/account.h"
#include "v8/group.h"
#include "v8/datastream.h"
#include "v8/photostream.h"
#include "v8/reply.h"

using namespace v8;



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

// Initialize the global object contains functions and objects
// like "Print" (function), "args" (object) or "Reply" (object)

void InitiliazeGlobalObject(v8::Isolate* i, v8::Handle<v8::ObjectTemplate> global, list_t* args) {
        // Bind the global 'print' function to the C++ Print callback.        
        global->Set(String::NewFromUtf8(i, "print"), FunctionTemplate::New(i, Print));
        
        // Bind the global 'read' function to the C++ Read callback.
        global->Set(String::NewFromUtf8(i, "read"), FunctionTemplate::New(i, Read));
        // Bind the global 'load' function to the C++ Load callback.
        global->Set(String::NewFromUtf8(i, "load"), FunctionTemplate::New(i, Load));
        // Bind the 'quit' function
        //global->Set(String::NewFromUtf8(i, "quit"), FunctionTemplate::New(i, Quit));
        global->Set(String::NewFromUtf8(i, "exit"), FunctionTemplate::New(i, Exit));
        // Bind the 'version' function
        global->Set(String::NewFromUtf8(i, "version"), FunctionTemplate::New(i, Version));
        
        // Add Account/Group/Datastream/... constructors
        
        Point::JS::Register(global, i);
        Account::JS::Register(global, i);                
        Group::JS::Register(global, i);
        Datastream::JS::Register(global, i);
        Photostream::JS::Register(global, i);
        
        // Set the "args" object which contains arguments passed by url
        
        global->Set(String::NewFromUtf8(i, "args"), createArgs(i, args));
        
        // Set the Reply object wich contains the body and the content type of the request
        
        Handle<ObjectTemplate> reply_obj = Reply::JS::GetNewTemplate(i);
        global->Set(String::NewFromUtf8(i, "Reply"), reply_obj);
}


void DeleteAllCppObjects() {
        //Point::JS::DeleteAllRef();
        Datastream::JS::DeleteAllRef();
        Photostream::JS::DeleteAllRef();
        Group::JS::DeleteAllRef();
        Account::JS::DeleteAllRef();
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


char* server_execute_script(const char* code, list_t* args, char** content_type) {
        
        char * replycontent = NULL;
        *content_type = NULL;

        if (!code) {
                fprintf(stderr, "Empty source code\n");
                return GetError(ScriptError::EMPTY);
        }

        v8::Isolate* isolate = v8::Isolate::New();
        
        {
                v8::Isolate::Scope iscope(isolate);
                //v8::V8::Initialize();
                //v8::Locker l(isolate);

                v8::HandleScope handle_scope(isolate);
                //v8::Local<v8::Context> context = v8::Context::New(isolate);
                
                // Create a template for the global object.
                Handle<v8::ObjectTemplate> global = ObjectTemplate::New(isolate);
                InitiliazeGlobalObject(isolate, global, args);
                
                Handle<Context> context = Context::New(isolate, NULL, global);
                
                if (!context.IsEmpty()) {

                        v8::Context::Scope context_scope(context);
                        v8::TryCatch try_catch;
                        
                        Handle<String> source = String::NewFromUtf8(isolate, code);
                        Handle<Script> script = Script::Compile(source);
                        
                        // Compilation error
                        if (try_catch.HasCaught()) {
                                String::Utf8Value exception(try_catch.Exception());
                                replycontent = GetError(ScriptError::COMPILESCRIPT, *exception);
                        } else {
                                Handle<Value> result = script->Run();

                                // Execution error
                                if (try_catch.HasCaught()) {

                                        String::Utf8Value exception(try_catch.Exception());
                                        if (*exception) {

                                                // Script ended with "exit()" function
                                                if (strcmp(*exception, "Exit") == 0) {
                                                        
                                                        const std::ostringstream& stream = Reply::GetStream();
                                                        const std::string& str = stream.str();
                                                        
                                                        replycontent = strdup(str.c_str());
                                                        
                                                } else {
                                                        replycontent = GetError(ScriptError::RUNSCRIPT, *exception);
                                                }
                                                
                                        } else {
                                                replycontent = GetError(ScriptError::RUNSCRIPT);
                                        }
                                } else {
                                        const std::ostringstream& stream = Reply::GetStream();
                                        const std::string& str = stream.str();

                                        replycontent = strdup(str.c_str());
                                }
                                
                        }
                        
                } else {
                        replycontent =  GetError(ScriptError::CONTEXT);
                }

                //context->Dispose();
                
                //Reply::Print();
                Reply::Clear();

                // Delete all "C++ wrapped objects"
                
                DeleteAllCppObjects();

                // Set content_type

                const std::string& mime = Reply::GetMimeType();

                if (!mime.empty()) {
                        *content_type = strdup(mime.c_str());
                } else {
                        *content_type = 0;
                }

                
                context.Clear();
        }

        isolate->Dispose();

        return replycontent;
}

void server_free_result(char* buffer)
{
        if (buffer) {
                free(buffer);
        }
}
