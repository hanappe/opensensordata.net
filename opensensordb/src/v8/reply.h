#ifndef REPLYV8_H
#define REPLYV8_H

#include <v8.h>

#include <sstream>

using namespace v8;

class Reply {

public:

        Reply() { }
	~Reply() { }

        static void Append(const char * s);
        static void Append(const std::string& s);
        static void AppendLine(const char * s);
        static void AppendLine(const std::string& s);

        static void Print();
        static void Clear();

        static const std::ostringstream& GetStream();
        static void SetMimeType(const char * mt);
        static const std::string& GetMimeType();

        static std::ostringstream oss;
        static std::string mime_type; 

        struct JS {

                static Handle<ObjectTemplate> GetNewTemplate(Isolate * i);
 
               // Register function wich create javascript object "Reply" in global scope
                static void Register(Handle<ObjectTemplate> global, Isolate* i);
                static void Constructor(const FunctionCallbackInfo<Value>& info);
                static void Write(const FunctionCallbackInfo<Value>& info);
                static void WriteLine(const FunctionCallbackInfo<Value>& info);
                static void Print(const FunctionCallbackInfo<Value>& info);
                static void SetMimeType(const FunctionCallbackInfo<Value>& info);
        
        }; // end struct JS
};

#endif // REPLYV8_H

