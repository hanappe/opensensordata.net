#include "jsengine.h"

using namespace v8;

int main(int argc, char* argv[]) {

        //jsengine_main(argc, argv);

        // Create a new Isolate and make it the current one.
        Isolate* isolate = Isolate::New();
        Isolate::Scope isolate_scope(isolate);

        // Create a stack-allocated handle scope.
        HandleScope handle_scope(isolate);

        // Create a new context.
        Handle<Context> context = Context::New(isolate);

        // Enter the context for compiling and running the hello world script.
        Context::Scope context_scope(context);

        // Create a string containing the JavaScript source code.
        Handle<String> source = String::NewFromUtf8(isolate, "var a = 'test';'Hello' + ', World!'");

        // Compile the source code.
        Handle<Script> script = Script::Compile(source);

        // Run the script to get the result.
        Handle<Value> result = script->Run();

        // Convert the result to an UTF8 string and print it.
        String::Utf8Value utf8(result);
        printf("%s\n", *utf8);

        // TEST

        Handle<String> sourceb = String::NewFromUtf8(isolate, "var a = 'test';JSON.stringify(a);");

        // Compile the source code.
        Handle<Script> scriptb = Script::Compile(sourceb);

        // Run the script to get the result.
        Handle<Value> resultb = scriptb->Run();

        // Convert the result to an UTF8 string and print it.
        String::Utf8Value utf8b(resultb);
        printf("%s\n", *utf8b);

        return 0;
}

