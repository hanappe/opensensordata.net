#include "account.h"

#include "db.h"
#include "group.h"

bool Account::loadFromId(int id) {
        clear();
        //return false;
        return DB::GetInstance()->loadAccount(this, id);
}

bool Account::loadFromUsername(int id) {
        clear();
        return false;
        //return DB::GetInstance()->loadAccount(this, id);
}

void Account::print() {
        std::cout << "Account[id: " << id
                  << " username " << username
                  << " email " << email
                  << "]"<< std::endl;        
}

void Account::clear() {
        for (std::vector<Group*>::iterator it = groups.begin(); it != groups.end(); ++it) {
                delete (*it);
        }
}

Handle<Array> Account::select(Isolate * iso, time_t start, time_t end) const {

        //std::cout << "Datastream::select2: " << std::endl;
        Handle<Array> main_array = Array::New(iso, 0);

        //std::cout << "data row size: " << datarows.size() << std::endl;                
        size_t group_count = 0;
        for (std::vector<Group*>::const_iterator it = groups.begin(); it != groups.end(); ++it) {
                const Group * g = (*it);
       
                Handle<Array> group_array = g->select(iso, start, end);
                main_array->Set(group_count, group_array);
                group_count++;
        }

        //std::cout << "Datastream::select2 datarow A result: " << datarows_count << std::endl;
        return main_array;
}


Local<Object> Account::JS::GetNewInstance(Isolate * i) {

        Handle<ObjectTemplate> account_template = ObjectTemplate::New();
        account_template->SetInternalFieldCount(1);
                        
        // set a javascript function
        account_template->Set(String::NewFromUtf8(i, "load"), FunctionTemplate::New(i, Load));
        
        return account_template->NewInstance();
}

void Account::JS::SetupObject(Local<Object> obj, Account * a, Isolate* i) {

        if (!a) {
                fprintf(stderr, "Account::JS::SetupObject error, Account is null");
                return;
        }

        obj->Set(String::NewFromUtf8(i, "id"), Number::New(i, a->id));
        obj->Set(String::NewFromUtf8(i, "username"), String::NewFromUtf8(i, a->username));
        obj->Set(String::NewFromUtf8(i, "email"), String::NewFromUtf8(i, a->email));

        // Alias
        std::vector<Group*>& groups = a->groups;
        
        Handle<Array> groups_array = Array::New(i, groups.size());
        
        int group_count = 0;
        for (std::vector<Group*>::iterator it = groups.begin(); it != groups.end(); ++it, group_count++) {
                
                Group * group = (*it);                
                
                Local<Object> g_obj = Group::JS::GetNewInstance(i);
                g_obj->SetInternalField(0, External::New(i, group));
          
                Group::JS::SetupObject(g_obj, group, i); 

                groups_array->Set(group_count, g_obj);
        }

        obj->Set(String::NewFromUtf8(i, "groups"), groups_array);
        
}

void Account::JS::Register(Handle<ObjectTemplate> global, Isolate* i) {
        global->Set(String::NewFromUtf8(i, "Account"), FunctionTemplate::New(i, Constructor));
}


void Account::JS::Constructor(const FunctionCallbackInfo<Value>& info) {
        v8::Isolate* i = v8::Isolate::GetCurrent();
                
        Account * acc = new Account();
        if (acc) {
                Local<Object> obj = GetNewInstance(i);
                obj->SetInternalField(0, External::New(i, acc));
                // Create and Return this newly created object
                info.GetReturnValue().Set(obj);
        }
}

void Account::JS::Load(const FunctionCallbackInfo<Value>& info ) {
        
        Isolate * i = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Account * account = static_cast<Account*>(ptr);

        const Handle<Value>& arg1 = info[0]; 
        
        bool loaded = false;
        if (!arg1.IsEmpty()) {
                /*
                if (arg1.IsString()) {
                        // TODO
                        char * username = "placeholder";
                        //loaded = account->loadFromUserName(username);
                } else
                */ {
                        const int account_id = !arg1.IsEmpty() ? arg1->Int32Value() : 0;
                        loaded = account->loadFromId(account_id);
                }
        }

        if (loaded) {
                // Fill cpp object from database
                Account::JS::SetupObject(info.This(), account, i);
        } else {
                std::cerr << "Account loading error (bad js argument)" << std::endl;
        }
}

void Account::JS::Select(const FunctionCallbackInfo<Value>& info)  {

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Account * account = static_cast<Account*>(ptr);
        
        Handle<Value> arg1 = info[0];
        Handle<Value> arg2 = info[1];
        
        Handle<String> inv = String::NewFromUtf8(isolate, "Invalid Date");

        bool ok = !arg1.IsEmpty() && !arg2.IsEmpty();
        ok = ok && !arg1->IsUndefined() && !arg2->IsUndefined();
        ok = ok && arg1->IsDate() && arg2->IsDate();

        // Convert args to string value to check if date is valid        
        {
                const v8::String::Utf8Value invalid_date(inv);

                v8::String::Utf8Value str_arg1(info[0]);
                v8::String::Utf8Value str_arg2(info[1]);
                ok = ok && strcmp(*str_arg1, *invalid_date) != 0 && strcmp(*str_arg2, *invalid_date) != 0;
        }

        if (ok) {
                const double start_t = arg1->NumberValue();
                const time_t start_ut = (time_t)start_t / 1000;
                const double end_t = arg2->NumberValue();
                const time_t end_ut = (time_t)end_t / 1000;

                Handle<Array> account_array = account->select(isolate, start_ut, end_ut);
                info.GetReturnValue().Set(account_array);
        }
}
