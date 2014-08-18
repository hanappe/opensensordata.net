#include "reply.h"

#include <iostream>
#include <cstring>

std::ostringstream Reply::oss;
std::string Reply::mime_type = std::string("text/plain");

void Reply::Append(const char * s) {
        oss << s;
}
void Reply::Append(const std::string& s) {
        oss << s;
}

void Reply::AppendLine(const char * s) {
        oss << s << "\n";
}

void Reply::AppendLine(const std::string& s) {
        oss << s << "\n";
}

void Reply::Print() {
        std::cout << "Reply[content:\"" << oss.str() << "\", mime:\"" << mime_type << "\"]" << std::endl;        
}

void Reply::Clear() {
        oss.str("");
        oss.clear();
}

const std::ostringstream& Reply::GetStream() {
        return oss;
}

void Reply::SetMimeType(const char * mt) {
        if (mt) {
                mime_type = std::string(mt);
        }
}

const std::string& Reply::GetMimeType() {
        return mime_type;
}

Handle<ObjectTemplate> Reply::JS::GetNewTemplate(Isolate * i) {

        Handle<ObjectTemplate> reply_template = ObjectTemplate::New(i);
        //reply_template->SetInternalFieldCount(1);
                        
        // set a javascript function
        reply_template->Set(String::NewFromUtf8(i, "write"), FunctionTemplate::New(i, Write));
        reply_template->Set(String::NewFromUtf8(i, "writeln"), FunctionTemplate::New(i, WriteLine));
        reply_template->Set(String::NewFromUtf8(i, "print"), FunctionTemplate::New(i, Print));
        reply_template->Set(String::NewFromUtf8(i, "setMimeType"), FunctionTemplate::New(i, SetMimeType));

        return reply_template;
}


void Reply::JS::Register(Handle<ObjectTemplate> global, Isolate* i) {
        global->Set(String::NewFromUtf8(i, "Reply"), FunctionTemplate::New(i, Constructor));
}


void Reply::JS::Constructor(const FunctionCallbackInfo<Value>& info) {
        v8::Isolate* i = v8::Isolate::GetCurrent();
                
        //Reply * acc = new Reply();
        //if (acc) {
        //Local<Object> obj = GetNewInstance(i);
                
                //obj->SetInternalField(0, External::New(i, acc));
                // Create and Return this newly created object
        //      info.GetReturnValue().Set(obj);
                //}
}

namespace {

        bool isquote(char c) {
                return c == '\"';
        }

        const char* trim_quote(char *str) {
                char *end;

                // Trim leading space
                while(isquote(*str)) str++;

                if(*str == 0)  // Empty ?
                        return str;

                // Trim trailing quote
                end = str + std::strlen(str) - 1;
                while(end > str && isquote(*end)) end--;

                // Write new null terminator
                *(end + 1) = 0;

                return str;
        }


        void to_json(Isolate * i, Handle<Value> value, std::string& result, bool unquote = false) {
                //Isolate * i = Isolate::GetCurrent();
                HandleScope scope(i);
                TryCatch try_catch;
                Handle<Object> global = i->GetCurrentContext()->Global();
                //Handle<Object> global = Context::GetCurrent()->Global();
                Handle<Object> json = Handle<Object>::Cast(global->Get(String::NewFromUtf8(i, "JSON")));
                Handle<Function> func = Handle<Function>::Cast(json->GetRealNamedProperty(String::NewFromUtf8(i, "stringify")));
                Handle<Value> args[1];
                args[0] = value;
                String::Utf8Value ret(func->Call(global, 1, args)->ToString());

                result = unquote ? trim_quote(*ret) : *ret;
        }

        Handle<Value> from_json(Isolate * i, std::string str) {
                HandleScope scope(Isolate::GetCurrent());
                TryCatch try_catch;

                Handle<Object> global = i->GetCurrentContext()->Global();
                Handle<Object> json = Handle<Object>::Cast(global->Get(String::NewFromUtf8(i, "JSON")));
                Handle<v8::Function> func = Handle<Function>::Cast(json->GetRealNamedProperty(String::NewFromUtf8(i, "parse")));
                Handle<v8::Value> args[1];
                
                args[0] = String::NewFromUtf8(i, str.c_str());
                return func->Call(global, 1, args);
        }
        
};

void Reply::JS::Write(const FunctionCallbackInfo<Value>& info) {

        Isolate * isolate = info.GetIsolate();
        
        const Handle<Value>& arg1 = info[0];
        

        if (!arg1.IsEmpty()) {
                std::string result;
                to_json(isolate, arg1, result);
                AppendLine(result);
        }
}

void Reply::JS::WriteLine(const FunctionCallbackInfo<Value>& info) {

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        const Handle<Value>& arg1 = info[0];
        

        if (!arg1.IsEmpty()) {
                std::string result;
                to_json(isolate, arg1, result);
                AppendLine(result);
        }
}

void Reply::JS::Print(const FunctionCallbackInfo<Value>& info) {
        Reply::Print();
}

void Reply::JS::SetMimeType(const FunctionCallbackInfo<Value>& info) {

        Isolate * isolate = info.GetIsolate();
                
        const Handle<Value>& arg1 = info[0];
        
        if (!arg1.IsEmpty() && arg1->IsString()) {
                std::string result;
                to_json(isolate, arg1->ToString(), result, true);
                Reply::SetMimeType(result.c_str());
        }
}
