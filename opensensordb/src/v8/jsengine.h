#ifndef JSENGINE_H
#define JSENGINE_H

#include <v8.h>

v8::Handle<v8::Context> CreateShellContext(v8::Isolate* isolate);
void RunShell(v8::Handle<v8::Context> context);
int RunMain(v8::Isolate* isolate, int argc, char* argv[]);
bool ExecuteString(v8::Isolate* isolate,
                   v8::Handle<v8::String> source,
                   v8::Handle<v8::Value> name,
                   bool print_result,
                   bool report_exceptions);

bool ExecuteString2(v8::Isolate* isolate,
                   v8::Handle<v8::String> source,
                   v8::Handle<v8::Value> name,
                   v8::Handle<v8::Value>& result,
                   bool print_result,
                   bool report_exceptions);


void Print(const v8::FunctionCallbackInfo<v8::Value>& args);
void Read(const v8::FunctionCallbackInfo<v8::Value>& args);
void Load(const v8::FunctionCallbackInfo<v8::Value>& args);
void Quit(const v8::FunctionCallbackInfo<v8::Value>& args);
void Version(const v8::FunctionCallbackInfo<v8::Value>& args);
v8::Handle<v8::String> ReadFile(v8::Isolate* isolate, const char* name);
void ReportException(v8::Isolate* isolate, v8::TryCatch* handler);

const char* ToCString(const v8::String::Utf8Value& value);

static bool run_shell;

int jsengine_main(int argc, char * argv[]);

#endif //JSENGINE_H
