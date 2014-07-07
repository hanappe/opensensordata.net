#include "group.h"

#include "db.h"
#include "datastream.h"
#include "photostream.h"


bool Group::load(int id) {
        return DB::GetInstance()->loadGroup(this, id);
}

void Group::print() {
        std::cout << "Group[id: " << id
                  << " ownerId " << ownerId
                  << " name " << name
                  << "]"<< std::endl;
}

void Group::clear() {

        for (std::vector<Datastream*>::iterator it = datastreams.begin(); it != datastreams.end(); ++it) {
                delete (*it);
        }
        
        for (std::vector<Photostream*>::iterator it = photostreams.begin(); it != photostreams.end(); ++it) {
                delete (*it);
        }

}

Local<Object> Group::JS::GetNewInstance(Isolate * i) {

        Handle<ObjectTemplate> group_template = ObjectTemplate::New();
        group_template->SetInternalFieldCount(1);
                        
        // set a javascript function
        group_template->Set(String::NewFromUtf8(i, "load"), FunctionTemplate::New(i, Load));
        
        return group_template->NewInstance();
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
        
        std::cout << "Group::JS::SetupObject-datastream: " << datastreams.size() << std::endl;
        
        //return;
        Handle<Array> datastreams_array = Array::New(i, datastreams.size());
        
        int datastream_count = 0;
        for (std::vector<Datastream*>::iterator it = datastreams.begin(); it != datastreams.end(); ++it, datastream_count++) {
                
                Datastream * datastream = (*it);

                //Handle<Object> obj = Object::New(i);
                
                
                Local<Object> ds_obj = Datastream::JS::GetNewInstance(i);
                ds_obj->SetInternalField(0, External::New(i, datastream));
          
                Datastream::JS::SetupObject(ds_obj, datastream, i); 
                
                /*obj->PrototypeTemplate().Set(
                                            String::NewFromUtf8(i, "select"),
                                            Datastream::JS::Select
                                            //FunctionTemplate::New()->GetFunction()
                                            //FunctionTemplate::New(MyWheelsMethodCallback)->GetFunction();
                                            );
                */
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

                Local<Object> ps_obj = Photostream::JS::GetNewInstance(i);
                ps_obj->SetInternalField(0, External::New(i, photostream));
          
                Photostream::JS::SetupObject(obj, photostream, i); 
                
                datastreams_array->Set(photostream_count, obj);
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

                Local<Object> obj = GetNewInstance(i);
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
                        
                /*
                const std::vector<Datapoint>& datapoints_ = datastream->datapoints;
                int datapoint_count = 0;
                for (std::vector<Datapoint>::const_iterator itb = datapoints_.begin(); itb != datapoints_.end(); ++itb, datapoint_count++) {
                        
                        const Datapoint& d = (*itb);    
                        if (datastream_count < 2) {
                                //d.print();
                        }
                        
                        //Handle<Object> obj = Object::New(i);
                        //obj->Set(v8::String::NewFromUtf8(i, "date"), Date::New(i, (double)d.time * 1000));
                        //obj->Set(v8::String::NewFromUtf8(i, "value"), Number::New(i, d.value));
                        //datapoints_array->Set(datapoints_count, obj);
                        
                }
                */
                
                datastreams_array->Set(datastream_count, obj);
         
                //}
        }
        
        //self->Set(String::NewFromUtf8(i, "datastreams"), datastreams_array);

        //group->toObject(info.This(), isolate);

        info.This()->Set(String::NewFromUtf8(i, "datastreams"), datastreams_array);
}

