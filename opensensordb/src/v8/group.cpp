#include "group.h"

#include "db.h"
#include "datastream.h"
#include "photostream.h"

Group::Group(int i) : id(i) {
}

Group::~Group() {
        //clear();
}

bool Group::load(int id) {
        clear();
        return DB::GetInstance()->loadGroup(this, id);
}

void Group::print() {
        std::cout << "Group[id: " << id
                  << " ownerId " << ownerId
                  << " name " << name
                  << "]"<< std::endl;
}

void Group::clear() {
        //for (std::vector<Datastream*>::iterator it = datastreams.begin(); it != datastreams.end(); ++it) {
        //        delete (*it);
        //}
        
        //for (std::vector<Photostream*>::iterator it = photostreams.begin(); it != photostreams.end(); ++it) {
        //        delete (*it);
        //}

}

Handle<Array> Group::select(Isolate * iso, time_t start, time_t end) const {

        //std::cout << "Datastream::select2: " << std::endl;
        Handle<Array> main_array = Array::New(iso, 0);

        //std::cout << "data row size: " << datarows.size() << std::endl;                
        size_t datastream_count = 0;
        for (std::vector<Datastream*>::const_iterator it = datastreams.begin(); it != datastreams.end(); ++it) {
                const Datastream * ds = (*it);
                Handle<Array> datastream_array = ds->select(iso, start, end);
                main_array->Set(datastream_count, datastream_array);
        }

        //std::cout << "Datastream::select2 datarow A result: " << datarows_count << std::endl;

        return main_array;
}


Handle<ObjectTemplate> Group::JS::GetNewTemplate(Isolate * i) {
        Handle<ObjectTemplate> group_template = ObjectTemplate::New();
        group_template->SetInternalFieldCount(1);
        
        // set a javascript function
        group_template->Set(String::NewFromUtf8(i, "load"), FunctionTemplate::New(i, Load));
        group_template->Set(String::NewFromUtf8(i, "select"), FunctionTemplate::New(i, Select));
        
        return group_template;
}

void Group::JS::SetupObject(Local<Object> obj, Group * g, Isolate* i) {

        if (!g) {
                fprintf(stderr, "Group::JS::SetupObject error, Group is null");
                return;
        }

        g->print();

        obj->Set(String::NewFromUtf8(i, "id"), Number::New(i, g->id));
        obj->Set(String::NewFromUtf8(i, "ownerId"), Number::New(i, g->ownerId));
        obj->Set(String::NewFromUtf8(i, "name"), String::NewFromUtf8(i, g->name));

        
        // Alias
        std::vector<Datastream*>& datastreams = g->datastreams;
        
        Handle<Array> datastreams_array = Array::New(i, datastreams.size());
        
        int datastream_count = 0;
        for (std::vector<Datastream*>::iterator it = datastreams.begin(); it != datastreams.end(); ++it, datastream_count++) {
                
                Datastream * datastream = (*it);

                //Handle<Object> obj = Object::New(i);
                
                
                //Local<Object> ds_obj = Datastream::JS::GetNewInstance(i);

                Handle<ObjectTemplate> ds_template = Datastream::JS::GetNewTemplate(i);
                Local<Object> ds_obj = ds_template->NewInstance();

                ds_obj->SetInternalField(0, External::New(i, datastream));
          
                Datastream::JS::SetupObject(ds_obj, datastream, i); 
                
                //ds_template->Set(String::NewFromUtf8(i, "select"), FunctionTemplate::New(i, Select));
                datastreams_array->Set(datastream_count, ds_obj);
        }

        obj->Set(String::NewFromUtf8(i, "datastreams"), datastreams_array);
        
        
        
        // Photostream

        // Alias
        std::vector<Photostream*>& photostreams = g->photostreams;

        std::cout << "Group::JS::SetupObject-photostream: " << photostreams.size() << std::endl;
                
        Handle<Array> photostreams_array = Array::New(i, photostreams.size());
        
        int photostream_count = 0;
        for (std::vector<Photostream*>::iterator it = photostreams.begin(); it != photostreams.end(); ++it, photostream_count++) {
                Photostream * photostream = (*it);

                Handle<ObjectTemplate> ps_template = Photostream::JS::GetNewTemplate(i);
                Local<Object> ps_obj = ps_template->NewInstance();

                ps_obj->SetInternalField(0, External::New(i, photostream));
          
                Photostream::JS::SetupObject(ps_obj, photostream, i); 
                
                photostreams_array->Set(photostream_count, ps_obj);
        }

        obj->Set(String::NewFromUtf8(i, "photostreams"), photostreams_array);
}

void Group::JS::Register(Handle<ObjectTemplate> global, Isolate* i) {
        global->Set(String::NewFromUtf8(i, "Group"), FunctionTemplate::New(i, Constructor));
}


void Group::JS::Constructor(const FunctionCallbackInfo<Value>& info) {
        v8::Isolate* i = v8::Isolate::GetCurrent();
                
        const Handle<Value>& arg1 = info[0]; 
        
        const int group_id = !arg1.IsEmpty() && arg1->IsNumber() ? arg1->Int32Value() : 0;
  
        Group * group = new Group();
        
        if (group) {

                Handle<ObjectTemplate> g_template= GetNewTemplate(i);
                Local<Object> obj = g_template->NewInstance();
                obj->SetInternalField(0, External::New(i, group));
                // Create and Return this newly created object
                info.GetReturnValue().Set(obj);

        } else {
                fprintf(stderr, "group not found with id %d \n", group_id);
        }
}

void Group::JS::Load(const FunctionCallbackInfo<Value>& info ) {
        
        Isolate * i = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Group * group = static_cast<Group*>(ptr);

        const Handle<Value>& arg1 = info[0]; 
        
        const int group_id = !arg1.IsEmpty() ? arg1->Int32Value() : 0;

        // Fill cpp object from database

        group->load(group_id);

        Group::JS::SetupObject(info.This(), group, i);

        return;
        // Fill javascript object

        const std::vector<Datastream*>& datastreams = group->datastreams;
        
        Handle<Array> datastreams_array = Array::New(i, datastreams.size());
        
        int datastream_count = 0;
        for (std::vector<Datastream*>::const_iterator it = datastreams.begin(); it != datastreams.end(); ++it, datastream_count++) {
                
                const Datastream * datastream = (*it);
                        
                

                //Handle<Object> current_datastream = datastream->toObject();
                //Handle<Object> obj = Object::New(i);
                Handle<Object> obj = Object::New(i);
                //Datastream::JS::SetupObject(info.This(), datastream, i); 

                Datastream::JS::SetupObject(obj, datastream, i); 
                
                //obj->Set(v8::String::NewFromUtf8(i, "test1"), Date::New(i, 0));
                //obj->Set(v8::String::NewFromUtf8(i, "test2"), Number::New(i, 0));
                
                //datapoints_array->Set(datapoints_count, obj);
                                        
                datastreams_array->Set(datastream_count, obj);
         
                //}
        }
        
        //self->Set(String::NewFromUtf8(i, "datastreams"), datastreams_array);

        //group->toObject(info.This(), isolate);

        info.This()->Set(String::NewFromUtf8(i, "datastreams"), datastreams_array);
}



void Group::JS::Select(const FunctionCallbackInfo<Value>& info)  {

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Group * group = static_cast<Group*>(ptr);
        
        Handle<Value> arg1 = info[0];
        Handle<Value> arg2 = info[1];
        
        Handle<String> inv = String::NewFromUtf8(isolate, "Invalid Date");
               

        if (!group || group->datastreams.size() == 0) {
                return;
        }

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
                
                Handle<Array> datastreams_array = Array::New(isolate, group->datastreams.size());

                const double start_t = arg1->NumberValue();
                const long start_ut = (long)start_t / 1000;
                const double end_t = arg2->NumberValue();
                const long end_ut = (long)end_t / 1000;

                std::vector<Datastream*>& datastreams = group->datastreams;

                int datastream_count = 0;
                for (std::vector<Datastream*>::const_iterator i = datastreams.begin(); i != datastreams.end(); ++i, ++ datastream_count) {
                        Datastream * d = (*i);
                        Handle<Object> obj = Object::New(isolate);

                        std::cout << "Test: " << d->id << std::endl;

                        obj->Set(String::NewFromUtf8(isolate, "id"), Number::New(isolate, d->id));
                        obj->Set(String::NewFromUtf8(isolate, "name"), String::NewFromUtf8(isolate, d->name));

                        Handle<Array> selection = d->select(isolate, start_ut, end_ut);
                        obj->Set(String::NewFromUtf8(isolate, "datapoints"), selection);

                        datastreams_array->Set(datastream_count, obj);
                }

                info.GetReturnValue().Set(datastreams_array);
        }
}


std::vector<Group*> Group::JS::references;

void Group::JS::AddToRef(Group * group) {
        references.push_back(group);
}

void Group::JS::DeleteAllRef() {
        
        for (std::vector<Group*>::iterator it = references.begin(); it != references.end(); ++it) {
                delete (*it);
        }

        references.clear();        
}
