#include "jsengine.h"

#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "datastream.h"
#include "point.h"
#include "db.h"

#ifdef COMPRESS_STARTUP_DATA_BZ2
#error Using compressed startup data is not supported for this sample
#endif

/**
 * This sample program shows how to implement a simple javascript shell
 * based on V8.  This includes initializing V8 with command line options,
 * creating global functions, compiling and executing strings.
 *
 * For a more sophisticated shell, consider using the debug shell D8.
 */

using namespace v8;



class ShellArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
public:
        virtual void* Allocate(size_t length) {
                return memset(AllocateUninitialized(length), 0, length);
        }
        virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
        virtual void Free(void* data, size_t) { free(data); }
};



// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const v8::String::Utf8Value& value) {
        return *value ? *value : "<string conversion failed>";
}


// Creates a new execution environment containing the built-in
// functions.
v8::Handle<v8::Context> CreateShellContext(v8::Isolate* isolate) {
        // Create a template for the global object.
        v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
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
        
        return v8::Context::New(isolate, NULL, global);
}


// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called.  Prints its arguments on stdout separated by
// spaces and ending with a newline.
void Print(const v8::FunctionCallbackInfo<v8::Value>& args) {
        bool first = true;
        for (int i = 0; i < args.Length(); i++) {
                v8::HandleScope handle_scope(args.GetIsolate());
                if (first) {
                        first = false;
                } else {
                        printf(" ");
                }
                v8::String::Utf8Value str(args[i]);
                const char* cstr = ToCString(str);
                printf("%s", cstr);
        }
        printf("\n");
        fflush(stdout);
}


// The callback that is invoked by v8 whenever the JavaScript 'read'
// function is called.  This function loads the content of the file named in
// the argument into a JavaScript string.
void Read(const v8::FunctionCallbackInfo<v8::Value>& args) {
        if (args.Length() != 1) {
                args.GetIsolate()->ThrowException(
                                                  v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters"));
                return;
        }
        v8::String::Utf8Value file(args[0]);
        if (*file == NULL) {
                args.GetIsolate()->ThrowException(
                                                  v8::String::NewFromUtf8(args.GetIsolate(), "Error loading file"));
                return;
        }
        v8::Handle<v8::String> source = ReadFile(args.GetIsolate(), *file);
        if (source.IsEmpty()) {
                args.GetIsolate()->ThrowException(
                                                  v8::String::NewFromUtf8(args.GetIsolate(), "Error loading file"));
                return;
        }
        args.GetReturnValue().Set(source);
}


// The callback that is invoked by v8 whenever the JavaScript 'load'
// function is called.  Loads, compiles and executes its argument
// JavaScript file.
void Load(const v8::FunctionCallbackInfo<v8::Value>& args) {
        for (int i = 0; i < args.Length(); i++) {
                v8::HandleScope handle_scope(args.GetIsolate());
                v8::String::Utf8Value file(args[i]);
                if (*file == NULL) {
                        args.GetIsolate()->ThrowException(
                             v8::String::NewFromUtf8(args.GetIsolate(), "Error loading file"));
                        return;
                }
                v8::Handle<v8::String> source = ReadFile(args.GetIsolate(), *file);
                if (source.IsEmpty()) {
                        args.GetIsolate()->ThrowException(
                            v8::String::NewFromUtf8(args.GetIsolate(), "Error loading file"));
                        return;
                }
                if (!ExecuteString(args.GetIsolate(),
                                   source,
                                   v8::String::NewFromUtf8(args.GetIsolate(), *file),
                                   false,
                                   false)) {
                        args.GetIsolate()->ThrowException(
                             v8::String::NewFromUtf8(args.GetIsolate(), "Error executing file"));
                        return;
                }
        }
}


// The callback that is invoked by v8 whenever the JavaScript 'quit'
// function is called.  Quits.
void Quit(const v8::FunctionCallbackInfo<v8::Value>& args) {
        // If not arguments are given args[0] will yield undefined which
        // converts to the integer value 0.
        int exit_code = args[0]->Int32Value();
        fflush(stdout);
        fflush(stderr);
        exit(exit_code);
}


void Version(const v8::FunctionCallbackInfo<v8::Value>& args) {
        args.GetReturnValue().Set(
             v8::String::NewFromUtf8(args.GetIsolate(), v8::V8::GetVersion()));
}


// Reads a file into a v8 string.
v8::Handle<v8::String> ReadFile(v8::Isolate* isolate, const char* name) {
        FILE* file = fopen(name, "rb");
        if (file == NULL) return v8::Handle<v8::String>();
        
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        rewind(file);
        
        char* chars = new char[size + 1];
        chars[size] = '\0';
        for (int i = 0; i < size;) {
                int read = static_cast<int>(fread(&chars[i], 1, size - i, file));
                i += read;
        }
        fclose(file);
        v8::Handle<v8::String> result =
                v8::String::NewFromUtf8(isolate, chars, v8::String::kNormalString, size);
        delete[] chars;
        return result;
}


// Process remaining command line arguments and execute files
int RunMain(v8::Isolate* isolate, int argc, char* argv[]) {
        for (int i = 1; i < argc; i++) {
                const char* str = argv[i];
                if (strcmp(str, "--shell") == 0) {
                        run_shell = true;
                } else if (strcmp(str, "-f") == 0) {
                        // Ignore any -f flags for compatibility with the other stand-
                        // alone JavaScript engines.
                        continue;
                } else if (strncmp(str, "--", 2) == 0) {
                        fprintf(stderr,
                                "Warning: unknown flag %s.\nTry --help for options\n", str);
                } else if (strcmp(str, "-e") == 0 && i + 1 < argc) {
                        // Execute argument given to -e option directly.
                        v8::Handle<v8::String> file_name =
                                v8::String::NewFromUtf8(isolate, "unnamed");
                        v8::Handle<v8::String> source =
                                v8::String::NewFromUtf8(isolate, argv[++i]);
                        if (!ExecuteString(isolate, source, file_name, false, true)) return 1;
                } else {
                        // Use all other arguments as names of files to load and run.
                        v8::Handle<v8::String> file_name = v8::String::NewFromUtf8(isolate, str);
                        v8::Handle<v8::String> source = ReadFile(isolate, str);
                        if (source.IsEmpty()) {
                                fprintf(stderr, "Error reading '%s'\n", str);
                                continue;
                        }
                        if (!ExecuteString(isolate, source, file_name, false, true)) return 1;
                }
        }
        return 0;
}


// The read-eval-execute loop of the shell.
void RunShell(v8::Handle<v8::Context> context) {
        fprintf(stderr, "V8 version %s [sample shell]\n", v8::V8::GetVersion());
        static const int kBufferSize = 256;
        // Enter the execution environment before evaluating any code.
        v8::Context::Scope context_scope(context);
        v8::Local<v8::String> name(v8::String::NewFromUtf8(context->GetIsolate(), "(shell)"));
        while (true) {
                char buffer[kBufferSize];
                fprintf(stderr, "> ");
                char* str = fgets(buffer, kBufferSize, stdin);
                if (str == NULL) break;
                v8::HandleScope handle_scope(context->GetIsolate());
                ExecuteString(context->GetIsolate(),
                              v8::String::NewFromUtf8(context->GetIsolate(), str),
                              name,
                              true,
                              true);
        }
        fprintf(stderr, "\n");
}


// Executes a string within the current v8 context.
bool ExecuteString(v8::Isolate* isolate,
                   v8::Handle<v8::String> source,
                   v8::Handle<v8::Value> name,
                   bool print_result,
                   bool report_exceptions) {
        v8::HandleScope handle_scope(isolate);
        v8::TryCatch try_catch;
        v8::ScriptOrigin origin(name);
        v8::Handle<v8::Script> script = v8::Script::Compile(source, &origin);
        if (script.IsEmpty()) {
                // Print errors that happened during compilation.
                if (report_exceptions)
                        ReportException(isolate, &try_catch);
                return false;
        } else {
                v8::Handle<v8::Value> result = script->Run();
                if (result.IsEmpty()) {
                        assert(try_catch.HasCaught());
                        // Print errors that happened during execution.
                        if (report_exceptions)
                                ReportException(isolate, &try_catch);
                        return false;
                } else {
                        assert(!try_catch.HasCaught());
                        if (print_result && !result->IsUndefined()) {
                                // If all went well and the result wasn't undefined then print
                                // the returned value.
                                v8::String::Utf8Value str(result);
                                const char* cstr = ToCString(str);
                                printf("%s\n", cstr);
                        }
                        return true;
                }
        }
}


// Executes a string within the current v8 context.
bool ExecuteString2(v8::Isolate* isolate,
                    v8::Handle<v8::String> source,
                    v8::Handle<v8::Value> name,
                    v8::Handle<v8::Value>& resul,
                    bool print_result,
                    bool report_exceptions) {
        v8::HandleScope handle_scope(isolate);
        v8::TryCatch try_catch;
        v8::ScriptOrigin origin(name);
        v8::Handle<v8::Script> script = v8::Script::Compile(source);
        
        printf("ExecustreString2\n");
        
        if (script.IsEmpty()) {
                printf("ExecustreString2 A\n");
                // Print errors that happened during compilation.
                if (report_exceptions)
                        ReportException(isolate, &try_catch);
                return false;
        } else {
                printf("ExecustreString2 J\n");
                v8::Handle<v8::Value> result = script->Run();
                if (result.IsEmpty()) {
                        assert(try_catch.HasCaught());
                        // Print errors that happened during execution.
                        if (report_exceptions)
                                ReportException(isolate, &try_catch);
                        return false;
                } else {
                        printf("ExecustreString2 K\n"); 
                        assert(!try_catch.HasCaught());
                        printf("ExecustreString2 L %d %d\n", print_result, result->IsUndefined()); 
                        if (print_result && !result->IsUndefined()) {
                                // If all went well and the result wasn't undefined then print
                                // the returned value.
                                printf("ExecustreString2 M\n"); 
                                v8::String::Utf8Value str(result);
                                const char* cstr = ToCString(str);
                                printf("ExecustreString2 A : %s\n", cstr);
                        }
                        return true;
                }
        }
}


void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch) {
        v8::HandleScope handle_scope(isolate);
        v8::String::Utf8Value exception(try_catch->Exception());
        const char* exception_string = ToCString(exception);
        v8::Handle<v8::Message> message = try_catch->Message();
        if (message.IsEmpty()) {
                // V8 didn't provide any extra information about this error; just
                // print the exception.
                fprintf(stderr, "%s\n", exception_string);
        } else {
                // Print (filename):(line number): (message).
                v8::String::Utf8Value filename(message->GetScriptResourceName());
                const char* filename_string = ToCString(filename);
                int linenum = message->GetLineNumber();
                fprintf(stderr, "%s:%i: %s\n", filename_string, linenum, exception_string);
                // Print line of source code.
                v8::String::Utf8Value sourceline(message->GetSourceLine());
                const char* sourceline_string = ToCString(sourceline);
                fprintf(stderr, "%s\n", sourceline_string);
                // Print wavy underline (GetUnderline is deprecated).
                int start = message->GetStartColumn();
                for (int i = 0; i < start; i++) {
                        fprintf(stderr, " ");
                }
                int end = message->GetEndColumn();
                for (int i = start; i < end; i++) {
                        fprintf(stderr, "^");
                }
                fprintf(stderr, "\n");
                v8::String::Utf8Value stack_trace(try_catch->StackTrace());
                if (stack_trace.length() > 0) {
                        const char* stack_trace_string = ToCString(stack_trace);
                        fprintf(stderr, "%s\n", stack_trace_string);
                }
        }
}


Handle<Array> NewPointArray(int x, int y, int z) {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();

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

        return array;
        // Return the value through Close.
        //return handle_scope.Close(array);
}



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

                Point::JS::Register(global, isolate);
                Datastream::JS::Register(global, isolate);
        
        }

        // Create the global javascript variable "args"
        Handle<ObjectTemplate> createArgs(Isolate* isolate, const char * key, const char * value) {
                //HandleScope handle_scope(isolate);
                
                Handle<ObjectTemplate> args = ObjectTemplate::New(isolate);
                //Gl->SetInternalFieldCount(1);
                args->Set(String::NewFromUtf8(isolate, "quit"),
                          String::NewFromUtf8(isolate, "test"));
                
                //const char * key = "key";
                //const char * val = "value";
                
                args->Set(String::NewFromUtf8(isolate, key),
                          String::NewFromUtf8(isolate, value));
                
                return args;
        }


       void InitiliazeGlobalArgs(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global) {
               global->Set(String::NewFromUtf8(isolate, "args"), createArgs(isolate,"key", "value"));

        }

        void ReleaseV8() {
                v8::V8::Dispose();
        }
}


//char * jsengine_execute_script(char * script_path, __pair * args[], size_t n_args) {
int RunScript(v8::Isolate* isolate, const char* str) {
        v8::Handle<v8::String> file_name = v8::String::NewFromUtf8(isolate, str);
        v8::Handle<v8::String> source = ReadFile(isolate, str);
        if (source.IsEmpty()) {
                fprintf(stderr, "Error reading '%s'\n", str);
                return 1;
        }
        if (!ExecuteString(isolate, source, file_name, false, true)) return 1;
        
        return 0;
}

//TODO REMOVE THIS FOLLOWING FUNCTION

int jsengine_main(int argc, char * argv[]) {
        
        InitializeV8();
        
        //v8::V8::SetFlagsFromCommandLine(&argc, argv, true);
        //ShellArrayBufferAllocator array_buffer_allocator;
        //v8::V8::SetArrayBufferAllocator(&array_buffer_allocator);
        v8::Isolate* isolate = GetIsolate();//v8::Isolate::New();
        
        int result;
        {


                v8::Isolate::Scope isolate_scope(isolate);
                v8::HandleScope handle_scope(isolate);
                
                // Create a template for the global object.
                //v8::Handle<v8::ObjectTemplate>& global = GetGlobal();//v8::ObjectTemplate::New(isolate);
                v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
                
                InitializeCustom(isolate, global);

                InitiliazeGlobalArgs(isolate, global);

                v8::Handle<v8::Context> context = v8::Context::New(isolate, NULL, global);

                if (!context.IsEmpty()) {

                        v8::Context::Scope context_scope(context);
                        
                        //std::cout << "Run Main" << std::endl;

                        if (argc > 1) {
                                //for (int i = 1; i < argc; i++) {
                                const char* str = argv[1];
                                
                                result = RunScript(isolate, str);
                                //}
                        } else {                        
                                //std::cout << "Run Shell: " << run_shell << std::endl;      
                                RunShell(context);
                        }
                        
                } else {
                        fprintf(stderr, "Error creating context\n");
                        result = 1;
                        
                }
                
        }


        ReleaseV8();

        return result;

}

//TODO REMOVE THIS FOLLOWING FUNCTION

int jsengine_execute_script(char * strpath) {

        if (!strpath || !strlen(strpath)) {
                fprintf(stderr, "Error strpath is invalide\n");
                return -1;
        }
        
        InitializeV8();
        
        v8::Isolate* isolate = GetIsolate();
        
        int result;
        {
                v8::Isolate::Scope isolate_scope(isolate);
                v8::HandleScope handle_scope(isolate);
                
                // Create a template for the global object.

                v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
                
                InitializeCustom(isolate, global);

                InitiliazeGlobalArgs(isolate, global);

                //initializeCustomTemplate(global, isolate);
                

                v8::Handle<v8::Context> context = v8::Context::New(isolate, NULL, global);

                //v8::Handle<v8::Context> context = CreateShellContext(isolate);
                if (!context.IsEmpty()) {

                        v8::Context::Scope context_scope(context);
                        
                        result = RunScript(isolate, strpath);
                        
                } else {
                        fprintf(stderr, "Error creating context\n");
                        result = 1;
                        
                }
                
        }

        ReleaseV8();

        return result;
}
